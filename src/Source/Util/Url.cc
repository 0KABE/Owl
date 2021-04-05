#include "Util/Url.hpp"

#include <uriparser/Uri.h>
#include <spdlog/spdlog.h>
#include <utility>

Owl::Url::Url(std::string url, std::string scheme, std::string host, std::string port,
              std::string path)
        : mUrl(std::move(url)), mScheme(std::move(scheme)), mHost(std::move(host)),
          mPort(std::move(port)), mPath(std::move(path)) {}

const std::string &Owl::Url::GetScheme() const { return mScheme; }

const std::string &Owl::Url::GetHost() const { return mHost; }

const std::string &Owl::Url::GetPort() const { return mPort; }

const std::string &Owl::Url::GetPath() const { return mPath; }

const std::string &Owl::Url::GetUrl() const { return mUrl; }

Owl::Url Owl::Url::Parse(const std::string &url) {
    UriUriA uri;
    const char *errorPos;
    int result = uriParseSingleUriA(&uri, url.c_str(), &errorPos);
    if (result == URI_SUCCESS && uri.scheme.first && uri.hostText.first) {
        std::string scheme(uri.scheme.first, uri.scheme.afterLast);
        std::string host = std::string(uri.hostText.first, uri.hostText.afterLast);
        std::string defaultPort = scheme == "https" ? "443" : "80";
        std::string port = uri.portText.first ? std::string(uri.portText.first,
                                                            uri.portText.afterLast)
                                              : defaultPort;
        std::string path =
                "/" + (uri.pathHead ? std::string(uri.pathHead->text.first) : "");
        return Url(url, scheme, host, port, path);
    }
    if (result != URI_SUCCESS)
        throw std::invalid_argument(
                fmt::format("Url parse failed. Url: {}", url));
    else if (!uri.scheme.first)
        throw std::invalid_argument(fmt::format("No url scheme: {}", url));
    else
        throw std::invalid_argument(fmt::format("No url host: {}", url));
}


