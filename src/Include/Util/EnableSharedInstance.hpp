#pragma once

#include <memory>
#include <type_traits>
#include "Util/Concepts.hpp"

namespace Owl {
    /**
     *
     * @tparam BaseClass
     *
     * @code
       class A : public EnableSharedInstance<A> {
       protected:
           A() = default;
       }
     * @endcode
     */
    template<typename BaseClass>
    class EnableSharedInstance {
    public:
        template<typename DerivedClass = BaseClass, typename ...Args>
        requires std::is_base_of_v<BaseClass, DerivedClass>
        static std::shared_ptr<BaseClass> NewInstance(Args &&...args) {
            return std::make_shared<EnableMakeShared < DerivedClass>>(std::forward<Args>(args)...);
        }

        template<typename Base>
        struct EnableMakeShared : public Base {
            template<typename ...Args>
            explicit EnableMakeShared(Args &&...args) : Base(std::forward<Args>(args)...) {}
        };
    };
}