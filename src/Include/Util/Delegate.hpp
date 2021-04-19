#pragma once

#include <functional>
#include <unordered_set>

template<typename ...Args>
class Delegate {
public:
    using Function = std::function<void(Args...)>;
    using SharedFunction = std::shared_ptr<Function>;

    Delegate &operator+=(const SharedFunction &function) {
        mSharedFunctions.insert(function);
        return *this;
    }

    Delegate &operator-=(const SharedFunction &function) {
        mSharedFunctions.erase(function);
        return *this;
    }

    Delegate &operator()(Args ... args) {
        for (const SharedFunction &function : mSharedFunctions)
            (*function)(std::forward<Args>(args)...);
        return *this;
    }

private:
    std::unordered_set<SharedFunction> mSharedFunctions;
};


