#pragma once

#include <functional>
#include <algorithm>
#include <utility>

namespace Owl {
    class FinalAction {
    public:
        using Action = std::function<void()>;

        FinalAction() = default;

        explicit FinalAction(Action action) : mActions({std::move(action)}) {}

        explicit FinalAction(std::vector<Action> actions) : mActions(std::move(actions)) {}

        ~FinalAction() {
            if (enable)
                std::for_each(mActions.begin(), mActions.end(), [=](const Action &action) { action(); });
        }

        void Disable() { enable = false; }

        FinalAction &operator+=(Action action) {
            mActions.push_back(std::move(action));
            return *this;
        }

    private:
        bool enable = true;
        std::vector<Action> mActions;
    };
}