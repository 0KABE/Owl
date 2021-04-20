#pragma once

#include "Util/Awaitable.hpp"
#include "Util/Buffer.hpp"
#include <spdlog/spdlog.h>
#include <boost/beast.hpp>
#include <utility>
#include "Url.hpp"
#include "ProxyNode.hpp"

namespace Owl {
    //TODO feature request: timeout detective
    template<typename RequestBody = boost::beast::http::empty_body, typename ResponseBody= boost::beast::http::dynamic_body>
    class HttpClient {
    public:
        using Verb = boost::beast::http::verb;
        using Request = boost::beast::http::request<RequestBody>;
        using Response = boost::beast::http::response<ResponseBody>;
        using Executor = const net::executor;

        HttpClient(Executor &executor, Url url, ProxyNode::ProxyPtr proxyNode)
                : mUrl(std::move(url)), mProxyNode(std::move(proxyNode)),
                  mOutbound(mProxyNode->GetOutbound(Endpoint(executor, mUrl.GetHost(), mUrl.GetPort()))) {}

        template<Verb HttpVerb = Verb::get, int HttpVersion = 10>
        HttpClient &BuildRequest(typename RequestBody::value_type body) {
            using fields = boost::beast::http::field;
            mRequest = Request(HttpVerb, mUrl.GetPath(), HttpVersion);
            mRequest.set(fields::host, mUrl.GetHost());
            mRequest.set(fields::user_agent, BOOST_BEAST_VERSION_STRING);
            mRequest.body() = std::move(body);
            return *this;
        }

        Awaitable<std::reference_wrapper<const Response>> Run() {
            co_await SendRequest();
            co_await ReceiveResponse();
            co_return mResponse;
        }

        void Stop() {
            mOutbound->Shutdown();
        }

        Awaitable<void> SendRequest() {
            spdlog::trace("Sending HTTP request to {} via {}", mUrl.GetUrl(), mProxyNode->GetName());
            Buffer buffer;
            std::ostream(&buffer) << mRequest;
            co_await mOutbound->Send(buffer);
        }

        Awaitable<void> ReceiveResponse() {
            spdlog::trace("Receiving HTTP response from {} via {}", mUrl.GetUrl(), mProxyNode->GetName());
            boost::beast::http::response_parser<boost::beast::http::dynamic_body> parser;
            Buffer tempBuffer;
            std::reference_wrapper<Buffer> buffer = tempBuffer;
            do {
                boost::beast::error_code errorCode;

                try { buffer = co_await mOutbound->Receive(); }
                catch (boost::system::system_error &systemError) {
                    if (systemError.code() != net::error::eof) {
                        throw boost::system::system_error(systemError);
                    }
                }

                auto const bytesParsed = parser.put(buffer.get().data(), errorCode);
                spdlog::trace("Receive {} bytes of HTTP response", bytesParsed);
                buffer.get().consume(bytesParsed);

                if (errorCode && errorCode != boost::beast::http::error::need_more) {
                    spdlog::error("Error when parse response: {}", errorCode.message());
                    throw boost::beast::system_error(errorCode);
                }
            } while (!parser.is_done());
            mResponse = parser.release();
            spdlog::trace("Parse HTTP response completed");
        }

    private :
        const Url mUrl;
        const ProxyNode::ProxyPtr mProxyNode;
        const Outbound::BoundPtr mOutbound;
        Request mRequest;
        Response mResponse;
    };
}


