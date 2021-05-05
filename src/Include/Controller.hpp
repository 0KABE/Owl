#pragma once

#include "Util/Singleton.hpp"
#include "Controller/Handler.hpp"

namespace Owl {
    //TODO set selected proxy
    //TODO get latency of proxies
    //TODO reload config
    class Controller : public Singleton<Controller> {
        friend Singleton<Controller>;
    public:
        using HandlerCollection = std::vector<HandlerPtr>;

        struct Register {
            explicit Register(HandlerPtr handler) {
                Controller &controller = Controller::GetInstance();
                controller.mHandlerCollection.push_back(std::move(handler));
            }
        };

        Owl::Awaitable<void>
        Handle(const Owl::Request &request, Owl::Socket socket, const Connection::Status &status) const;

    private:
        Controller() = default;

        HandlerCollection mHandlerCollection;
    };
}


