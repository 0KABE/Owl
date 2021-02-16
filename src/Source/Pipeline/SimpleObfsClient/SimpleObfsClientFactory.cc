#include "Pipeline/SimpleObfsClient/SimpleObfsClientFactory.hpp"

Owl::SimpleObfsClient::SimpleObfsClientPtr
Owl::SimpleObfsClientFactory::Build(const std::string &obfsType, const std::string &hostname) {
    auto it = mGenerator.find(obfsType);
    if (it != mGenerator.end())
        return it->second(hostname);
    throw std::runtime_error("No generator for " + obfsType);
}
