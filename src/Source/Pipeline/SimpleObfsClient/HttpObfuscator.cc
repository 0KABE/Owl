#include "Pipeline/SimpleObfsClient/HttpObfuscator.hpp"
#include "Pipeline/SimpleObfsClient/SimpleObfsClientFactory.hpp"
#include <boost/beast.hpp>
#include <spdlog/spdlog.h>


Owl::Buffer &Owl::HttpObfuscator::Obfuscate(Owl::Buffer &buffer) {
    using namespace boost::beast;
    static unsigned int curlMajorVersion = mRandomEngine() % 51;
    static unsigned int curlMinorVersion = mRandomEngine() % 2;

    if (mHandleOBFS) {
        mHandleOBFS = false;

        // generate web socket key
        std::string webSocketKey;
        webSocketKey.reserve(25);
        RandomBase64(webSocketKey);

        // construct request header
        http::request_header<> header;
        header.method(http::verb::get);
        header.target(mHostname);
        header.set(http::field::host, mHostname);
        header.set(http::field::user_agent, fmt::format("curl/7.{}.{}", curlMajorVersion, curlMinorVersion));
        header.set(http::field::upgrade, "websocket");
        header.set(http::field::connection, "Upgrade");
        header.set(http::field::sec_websocket_key, webSocketKey);
        header.set(http::field::content_length, to_static_string(buffer.size()));

        // serialize request header to string
        std::stringstream stringStream;
        stringStream << header;

        return Prepend(buffer, stringStream.str());
    } else {
        return buffer;
    }
}

Owl::Buffer &Owl::HttpObfuscator::DeObfuscate(Owl::Buffer &buffer) {
    if (mHandleDeOBFS) {
        std::string_view stringView(static_cast<const char *>(buffer.data().data()), buffer.size());
        size_t pos = stringView.find("\r\n\r\n");

        // return an emtpy buffer unless the buffer contains entire HTTP Header
        if (pos != std::string_view::npos) {
            mHandleDeOBFS = false;
            buffer.consume(pos + 4); // including \r\n\r\n
            return buffer;
        } else {
            return mDeObfuscateBuffer;// emtpy buffer
        }
    } else {
        return buffer;
    }
}

static auto reg = Owl::SimpleObfsClientFactory::Register<Owl::HttpObfuscator>("http");