//
// Created by C Dobrzycki on 19/12/2025.
//
#include <iostream>

#include "core/events/events.h"
#include "system/system.h"

// backend interface
namespace external_backend {
    struct rawClickEvent {
        struct {
            int x, y;
        } mouse_pos;

        int mouse_button;
    };

    // suppose backend has user-pointers and callbacks
    extern inline
    void* user_pointer = nullptr;

    void (*clickCallback)(rawClickEvent) = nullptr;

    void setClickCallback(void (*p)(rawClickEvent)) {
        clickCallback = p;
    }
}

// internal wrapper
namespace amber::backends::example {
    struct backend_t; // best to keep backend type undefined to use as a tag

    struct events { // best to keep as a type
        struct ClickEvent { // wrap external event type in a canonical internal type
            int x, y, button; // signals with semantic names
            explicit ClickEvent(const ::external_backend::rawClickEvent& e)
                : x(e.mouse_pos.x), y(e.mouse_pos.y), button(e.mouse_button) {
            }
        };

        // can also collect event types
        using all_events = util::type_list<ClickEvent>;
    };
}

// can allow introspection by mapping backend type to related namespaces
namespace amber::system {
    template<>
    struct backend_traits<backends::example::backend_t> {
        using events = backends::example::events;
    };
}

// can define backend-agnostic types
template<class backend>
struct A : amber::core::Listener<typename amber::system::backend_traits<backend>::events::ClickEvent> {
    using b_events = typename amber::system::backend_traits<backend>::events;

    A() {
        this->template addHandler<b_events::ClickEvent>(
            []<class E>(const E& e) requires amber::core::hasButton<E> {
                std::cout << "Event heard at agnostic subject! {" << e.button << "}" << std::endl;
            }
        );
    }
};

// can define reusable config types
template<class backend>
struct clicks_config {
    using listen_types = amber::util::type_list<
        typename amber::system::backend_traits<backend>::events::ClickEvent // ... and potentially more
    >;
};

// ... or can define backend-gnostic types
struct Obj : amber::core::Listener<clicks_config<amber::backends::example::backend_t>::listen_types> {
    // or without config - : amber::core::Listener<amber::backends::example::events::ClickEvent>
    using backend_t = amber::backends::example::backend_t;
    using events = amber::system::backend_traits<backend_t>::events;

    int captured = 0;
    A<backend_t> sub_obj; // can own without knowing listen types

    Obj() {
        this->addHandler<events::ClickEvent>(
            [&]<class E>(const E& e) requires amber::core::hasButton<E> {
                std::cout << "[" << captured++ << "] Primary heard mouse event! {" << e.button << "}" << std::endl;
            }
        );

        // backend wrapper type known, use functions from backend namespace
        // set user pointer
        external_backend::user_pointer = static_cast<void*>(this);
        // set callback
        external_backend::setClickCallback([](external_backend::rawClickEvent event) {
            events::ClickEvent e{event};
            Obj* primary_listener = nullptr;
            if constexpr (listens_to<events::ClickEvent>) {
                primary_listener = static_cast<Obj*>(external_backend::user_pointer);
                primary_listener->onEvent(e);
            }

            // maybe propagate
            if constexpr (decltype(sub_obj)::listens_to<events::ClickEvent>) {
                std::cout << "Propagate!" << std::endl;
                if (primary_listener) primary_listener->sub_obj.onEvent(e);
            }
        });

        // suppose backend dispatches events
        external_backend::clickCallback({{1, 2}, 1});
        external_backend::clickCallback({{1, 2}, 2});
        external_backend::clickCallback({{1, 2}, 1});
        external_backend::clickCallback({{1, 2}, 3});
    }
};

int main() {
    // user specifies concrete, internal backend type
    Obj o;
    return 0;
}
