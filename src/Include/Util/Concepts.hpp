#pragma once

#include <type_traits>

namespace Owl {
    template<typename BaseClass, typename DerivedClass>
    concept Derived = std::is_base_of_v<BaseClass, DerivedClass>
                      && !std::is_same_v<BaseClass, DerivedClass>;
}
