#pragma once

#include <type_traits>
#include <memory>
#include "Util/Awaitable.hpp"
#include "Util/Net.hpp"

namespace Owl {
    class Connection : public std::enable_shared_from_this<Connection> {
    public:
        using Speed = size_t;
        using Socket = net::ip::tcp::socket;
        using ConnectionWeakPtr = std::weak_ptr<Connection>;

        enum Status {
            UNINITIALIZED, OPEN, CLOSE
        };

        virtual void Open() = 0;

        void Close() { mStatus = CLOSE; }

        virtual ~Connection() = default;

        Speed GetUploadTraffic() const { return mUploadTraffic; }

        Speed GetDownloadTraffic() const { return mDownloadTraffic; }

        Awaitable<void> RefreshTraffic(std::chrono::milliseconds interval) {
            const net::executor &executor = co_await net::this_coro::executor;
            net::steady_timer timer(executor);
            while (mStatus != CLOSE) {
                timer.expires_after(interval);
                co_await timer.async_wait(use_awaitable);
                mUploadTraffic = mDownloadTraffic = 0;
            }
        }

    protected:
        Speed mUploadTraffic = 0;
        Speed mDownloadTraffic = 0;
        Status mStatus = UNINITIALIZED;
        ConnectionWeakPtr mConnectionWeakPtr;
    };

    template<class T>
    concept ConnectionDerived = std::is_base_of<Connection, T>::value && !std::is_same<Connection, T>::value;
}

