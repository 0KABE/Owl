#pragma once

#include "Util/Singleton.hpp"
#include "Controller/Handler.hpp"

namespace Owl {
    class Controller : public Singleton<Controller> {
        friend Singleton<Controller>;
    public:
        using HandlerCollection = std::vector<HandlerPtr>;

        template<typename T>
        struct Register {
            explicit Register(HandlerPtr handler) {
                Controller &controller = Controller::GetInstance();
                controller.mHandlerCollection.push_back(handler);
            }
        };

        Awaitable<void> Handle(const Request &request, Socket socket) const;

    private:
        Controller() = default;

        HandlerCollection mHandlerCollection;
    };
}


