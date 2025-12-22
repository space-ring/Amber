//
// Created by C Dobrzycki on 19/12/2025.
//
#include <iostream>

#include "core/events/events.h"

// read-only from backend
namespace external_backend {
    struct rawClickEvent {
        struct {
            int x, y;
        } mouse_pos;

        int mouse_button;
    };

    // suppose backend user-pointer and callback
    void* user_pointer = nullptr;

    void (*clickCallback)(rawClickEvent) = nullptr;

    void setClickCallback(void (*p)(rawClickEvent)) {
        clickCallback = p;
    }
}

namespace amber::backends {
    struct example_backend {
        struct events {
            struct ClickEvent { // wrap external event type in a canonical internal type
                int x, y, button; // signals with semantic names
                explicit ClickEvent(const ::external_backend::rawClickEvent& e)
                    : x(e.mouse_pos.x), y(e.mouse_pos.y), button(e.mouse_button) {
                }
            };

            // can invent events
            struct DummyEvent {
            };

            // can also collect event types
            using all = util::type_list<ClickEvent>;
        };
    };
}

// can define backend-agnostic types
template<class backend>
struct A : amber::core::Listener<typename backend::events::ClickEvent> {
    A() {
        this->template addHandler<backend::events::ClickEvent>(
            []<class E>(const E& e) requires amber::core::hasButton<E> {
                std::cout << "Event heard at agnostic subject! {" << e.button << "}" << std::endl;
            }
        );
    }
};

template<class backend>
struct Obj_config {
    using listen_types = amber::util::type_list<
        typename backend::events::ClickEvent
    >;
};

template<class>
struct Obj;

// or can define backend-gnostic types
template<>
struct Obj<amber::backends::example_backend> :
        amber::core::Listener<Obj_config<amber::backends::example_backend>::listen_types> {
    using backend_t = amber::backends::example_backend;

    int captured = 0;
    A<backend_t> sub_obj; // can own without knowing listen types

    Obj() {
        this->addHandler<backend_t::events::ClickEvent>(
            [&]<class E>(const E& e) requires amber::core::hasButton<E> {
                std::cout << "[" << captured++ << "] Primary heard mouse event! {" << e.button << "}" << std::endl;
            }
        );

        // backend wrapper type known, use functions from backend namespace
        // set user pointer
        external_backend::user_pointer = static_cast<void*>(this);
        // set callback
        external_backend::setClickCallback([](external_backend::rawClickEvent event) {
            backend_t::events::ClickEvent e{event};
            Obj* primary_listener = nullptr;
            if constexpr (listens_to<backend_t::events::ClickEvent>) {
                primary_listener = static_cast<Obj*>(external_backend::user_pointer);
                primary_listener->onEvent(e);
            }

            // maybe propagate
            if constexpr (decltype(sub_obj)::listens_to<backend_t::events::ClickEvent>) {
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
    // user specifies concrete backend type (as an amber wrapper)
    Obj<amber::backends::example_backend> o;
    return 0;
}
