//
// Created by croissant on 28/05/2022.
//

#ifndef ENGINE_VARIADIC_H
#define ENGINE_VARIADIC_H

#include <functional>

//template<class Return, class... Args>
//struct variadic {
//    const std::function<Return(Args...)> render;
//
//    variadic(const std::function<Return(Args...)>& render) : render(render) {}
//
//    Return operator()(Args... args) { if (active) return render(args...); }
//
//    bool active{true};
//};

template<class Return, class... Args>
class Variadic {
protected:
    const std::function<Return(Args...)> function;
public:
    bool active{true};

    Variadic(const std::function<Return(Args...)>& function) : function(function) {}

    virtual Return operator()(Args... args) { if (active) return function(args...); };
};
//template<class... Args>
//using renderMethod = variadic<void, Args...>;
//
//template<class Return, class... Args>
//using eventHandler = variadic<Return, Args...>;

//template<class Return, class... Args>
//Return variadic<Return, Args...>::operator()(Args... args) { return render(args...); }
//
//template<class Return, class... Args>
//variadic<Return, Args...>::variadic(const std::function<Return(Args...)>& render):render(render) {}

#endif //ENGINE_VARIADIC_H
