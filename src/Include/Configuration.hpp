#pragma once

#include "Util/Endpoint.hpp"
#include "Util/Net.hpp"
#include "Util/Singleton.hpp"
#include "Bound/Outbound.hpp"

namespace Owl {
    class Configuration {
    public:
        static void Load(const std::string &path);

        static Outbound::BoundPtr Match(Endpoint endpoint);
    };
}


