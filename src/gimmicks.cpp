#include "gimmicks.hpp"

template <class X>
X& singleton()
{
    static X x;
    return x;
}

std::unique_ptr<Gimmick> &gimmick_ptr() {
    return singleton<std::unique_ptr<Gimmick>>();
}

