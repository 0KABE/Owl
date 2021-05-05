#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/writer.h>
#include "Controller.hpp"
#include "Controller/PolicyHandler.hpp"
#include "Proxy/ProxyNodeManager.hpp"

Owl::Awaitable<void> Owl::PolicyHandler::Handle(const Owl::Request &request, Owl::Socket socket,
                                                const Owl::Connection::Status &status) const {
    using namespace rapidjson;
    using namespace boost::beast;
    ProxyNodeManager &proxyNodeManager = ProxyNodeManager::GetInstance();

    const ProxyNodeManager::Policies &policies = proxyNodeManager.GetPolicies();
    Document document(kObjectType);
    Value policiesJSON(kArrayType);
    for (const auto &policyPair: policies) {
        Value policy(kObjectType);

        Value name;
        name.SetString(policyPair.first.data(), policyPair.first.size(), document.GetAllocator());
        policy.AddMember("name", name, document.GetAllocator());
        Value selected;
        selected.SetString(policyPair.second->GetSelectedProxy()->GetName().data(),
                           policyPair.second->GetSelectedProxy()->GetName().size(),
                           document.GetAllocator());
        policy.AddMember("selected", selected, document.GetAllocator());
        Value type;
        type.SetString(policyPair.second->GetPolicyType().data(),
                       policyPair.second->GetPolicyType().length(),
                       document.GetAllocator());
        policy.AddMember("type", type, document.GetAllocator());

        Value proxies(kArrayType);
        for (auto &proxy : policyPair.second->GetProxies()) {
            Value value;
            value.SetString(proxy->GetName().data(), proxy->GetName().length(),
                            document.GetAllocator());
            proxies.PushBack(value, document.GetAllocator());
        }

        policy.AddMember("proxies", proxies, document.GetAllocator());
        policiesJSON.PushBack(policy, document.GetAllocator());
    }

    document.AddMember("policies", policiesJSON, document.GetAllocator());
    StringBuffer stringBuffer;
    Writer writer(stringBuffer);
    document.Accept(writer);

    http::response<http::string_body> response;
    response.set(http::field::content_type, "application/json");
    response.set(http::field::content_length, std::to_string(stringBuffer.GetLength()));
    if (request.count(http::field::origin))
        response.set(http::field::access_control_allow_origin, "*");
    response.body() = stringBuffer.GetString();

    co_await async_write(socket, response, use_awaitable);
}

bool Owl::PolicyHandler::Match(const Owl::Request &request) const {
    return request.target() == TARGET;
}

[[maybe_unused]] static auto reg
        = Owl::Controller::Register(std::make_unique<Owl::PolicyHandler>());
