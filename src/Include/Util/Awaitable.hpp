#pragma once

#include "Util/Net.hpp"

namespace Owl {
    template<typename T>
    using Awaitable = net::awaitable<T>;
    using net::use_awaitable;
}
