#pragma once

#include <unordered_map>
#include "Pipeline/SimpleObfsClient.hpp"
#include "Util/Singleton.hpp"

namespace Owl {
    class SimpleObfsClientFactory : public Singleton<SimpleObfsClientFactory> {
        friend Singleton<SimpleObfsClientFactory>;
    public:
        using GeneratorFun = std::function<SimpleObfsClient::SimpleObfsClientPtr(const std::string &hostname)>;

        SimpleObfsClient::SimpleObfsClientPtr Build(const std::string &obfsType, const std::string &hostname);

        template<typename T>
        struct Register {
            explicit Register(const std::string &name) {
                SimpleObfsClientFactory &factory = GetInstance();
                auto it = factory.mGenerator.find(name);
                if (it != factory.mGenerator.end())
                    throw std::runtime_error(name + " has been registered");

                factory.mGenerator[name] = [](const std::string &hostname) {
                    return std::make_unique<T>(hostname);
                };
            }
        };

    private:
        std::unordered_map<std::string, GeneratorFun> mGenerator;
    };
}


