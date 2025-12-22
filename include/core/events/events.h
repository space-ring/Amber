//
// Created by C Dobrzycki on 16/12/2025.
//

#ifndef EVENTS_H
#define EVENTS_H

#include <utility>
#include <tuple>
#include <vector>
#include <functional>

#include "util/meta.h"

namespace amber::core {
    template<class Event>
    struct Handler {
        using function_t = std::function<void(const Event&)>;
        function_t function;

        template<class F>
            requires (!std::same_as<std::remove_cvref_t<F>, Handler>)
        explicit Handler(F&& handler) : function(handler) {
        }

        void operator()(const Event& event) {
            function(event);
        }
    };

    namespace detail {
        template<class... Events>
        struct Listener_impl {
            static_assert(sizeof...(Events) != 0, "AMBER USER ERROR: Cannot listen on zero events.");

            using listen_t = util::type_list<Events...>;

            template<class T>
            static constexpr bool listens_to = util::contains<T, Events...>;

            std::tuple<std::vector<Handler<Events>>...> handlers;

            template<class Event, class F>
                requires util::contains<Event, Events...> && std::invocable<F&, const Event&>
            unsigned addHandler(F&& handler) {
                auto& functions = std::get<std::vector<Handler<Event>>>(handlers);
                functions.emplace_back(std::forward<F>(handler));
                return static_cast<unsigned>(functions.size()) - 1;
            }

            template<class Event>
                requires util::contains<Event, Events...>
            void onEvent(const Event& event) {
                auto& functions = std::get<std::vector<Handler<Event>>>(handlers);
                for (auto& f: functions) {
                    f(event);
                }
            }
        };
    }

    template<class... T>
    struct Listener : detail::Listener_impl<T...> {
    };

    template<class... T>
    struct Listener<util::type_list<T...>> : detail::Listener_impl<T...> {
    };

    // common signals
    #define DECLARE_SIGNAL(concept_name, signal_name) \
    template<class E> concept concept_name = requires(const E& event) { event.signal_name; };

    DECLARE_SIGNAL(hasButton, button)
    DECLARE_SIGNAL(hasKey, key)
}


#endif //EVENTS_H
