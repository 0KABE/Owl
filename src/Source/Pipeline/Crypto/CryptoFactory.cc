#include "Pipeline/Crypto/CryptoFactory.hpp"

Owl::Pipeline::PipelinePtr Owl::CryptoFactory::Build(const std::string &method, const std::string &password) {
    auto it = generator.find(method);
    if (it != generator.end()) return it->second(password);
    throw std::runtime_error("no generator for " + method);
}
