#include <boost/beast.hpp>
#include "Controller.hpp"
#include "Controller/RuleHandler.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "Rule/RuleManager.hpp"

Owl::Awaitable<void>
Owl::RuleHandler::Handle(const Owl::Request &request, Owl::Socket socket, const Owl::Connection::Status &status) const {
    using namespace rapidjson;
    using namespace boost::beast;

    RuleManager &ruleManager = RuleManager::GetInstance();

    Document document;
    document.SetArray();
    for (const RulePtr &rulePtr : ruleManager.GetRuleCollection()) {
        Value ruleValue(kObjectType);

        ruleValue.AddMember("Type",
                            Value(rulePtr->RuleType().c_str(), rulePtr->RuleType().size(), document.GetAllocator()),
                            document.GetAllocator());

        ruleValue.AddMember("Condition",
                            Value(rulePtr->Condition().c_str(), rulePtr->Condition().size(), document.GetAllocator()),
                            document.GetAllocator());

        ruleValue.AddMember("Proxy",
                            Value(rulePtr->GetProxyPtr()->GetName().c_str(), rulePtr->GetProxyPtr()->GetName().size(),
                                  document.GetAllocator()), document.GetAllocator());

        document.PushBack(ruleValue, document.GetAllocator());
    }

    StringBuffer stringBuffer;
    Writer writer(stringBuffer);
    document.Accept(writer);

    http::response<http::string_body> response;
    response.set(http::field::content_type, "application/json");
    response.set(http::field::access_control_allow_origin, "*");
    response.body() = stringBuffer.GetString();

    co_await async_write(socket, response, use_awaitable);
}

bool Owl::RuleHandler::Match(const Owl::Request &request) const {
    return request.target() == TARGET && request.method() == Method::get;
}

[[maybe_unused]] static auto reg
        = Owl::Controller::Register(std::make_unique<Owl::RuleHandler>());
