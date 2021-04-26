#pragma once

#include <unordered_map>
#include "Pipeline/Crypto/Cipher.hpp"
#include "Util/Singleton.hpp"

namespace Owl {
    class CryptoFactory : public Singleton<CryptoFactory> {
        friend Singleton<CryptoFactory>;

    public:
        using GeneratorFun = std::function<PipelinePtr(const std::string &password)>;

        PipelinePtr Build(const std::string &method, const std::string &password);

        template<typename T>
        struct Register {
            explicit Register(const std::string &name) {
                CryptoFactory &cryptoFactory = GetInstance();
                auto it = cryptoFactory.generator.find(name);
                if (it != cryptoFactory.generator.end())
                    throw std::runtime_error(name + " has been registered.");

                cryptoFactory.generator[name] = [](const std::string &password) {
                    return std::make_unique<Cipher<T>>(password);
                };
            }
        };

    private:
        std::unordered_map<std::string, GeneratorFun> generator;
    };
}


