//
// Created by C Dobrzycki on 17/12/2025.
//

#ifndef META_H
#define META_H

#include <type_traits>

namespace amber::util {
    template<typename...>
    struct type_list;

    template<class T, class... Ts>
    concept contains = (std::is_same_v<T, Ts> || ...);

    namespace detail {
        template<class, class>
        struct forwards_impl;

        template<class... T, class... U>
        struct forwards_impl<type_list<T...>, type_list<U...>>
                : std::bool_constant<sizeof...(T) == sizeof...(U) && (std::constructible_from<T, U&&> && ...)> {
        };
    }

    template<class T, class U>
    concept forwards = detail::forwards_impl<T, U>::value;

    namespace detail {
        template<class Seen, class... Rest>
        struct unique_impl : std::true_type {
        };

        template<class... Seen, class Current, class... Rest>
        struct unique_impl<type_list<Seen...>, Current, Rest...>
                : std::bool_constant<
                    (!contains<Current, Seen...> &&
                        unique_impl<type_list<Seen..., Current>, Rest...>::value)
                > {
        };
    }

    template<class... Ts>
    concept unique = detail::unique_impl<type_list<>, Ts...>::value;
}

#endif //META_H
