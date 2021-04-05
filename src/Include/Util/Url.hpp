#pragma once

#include <string>

namespace Owl {
    class Url {
    public:
        static Url Parse(const std::string &url);

        [[nodiscard]] const std::string &GetScheme() const;

        [[nodiscard]] const std::string &GetHost() const;

        [[nodiscard]] const std::string &GetPort() const;

        [[nodiscard]] const std::string &GetUrl() const;

        [[nodiscard]] const std::string &GetPath() const;

    private:
        Url(std::string url, std::string scheme, std::string host, std::string port,
            std::string path);

    private:
        std::string mUrl;
        std::string mScheme;
        std::string mHost;
        std::string mPort;
        std::string mPath;
    };
}


