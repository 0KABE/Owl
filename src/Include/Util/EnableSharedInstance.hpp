#pragma once

#include <memory>
#include <type_traits>

namespace Owl {
    template<typename T>
    class EnableSharedInstance {
    public:
        template<typename ...Args>
        static std::shared_ptr<T> NewInstance(Args &&...args) {
            return std::make_shared<EnableMakeShared>(std::forward<Args>(args)...);
        }

        struct EnableMakeShared : public T {
            template<typename ...Args>
            explicit EnableMakeShared(Args &&...args) : T(std::forward<Args>(args)...) {}
        };
    };
}