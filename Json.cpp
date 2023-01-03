#include "Json.h"
#include "jsonValue.h"
#include "parser.h"
#include <memory>

Json::Json() :m_ptr(std::make_shared<JsonNull>()) {}
Json::Json(double value) :m_ptr(std::make_shared<JsonDouble>(value)) {}
Json::Json(int value) :m_ptr(std::make_shared<JsonInt>(value)) {}
Json::Json(bool value) :m_ptr(std::make_shared<JsonBoolean>(value)) {}
Json::Json(const std::string& value) : m_ptr(std::make_shared<JsonString>(value)) {}
Json::Json(const char* value) : m_ptr(std::make_shared<JsonString>(value)) {}
Json::Json(const Json::array& values) : m_ptr(std::make_shared<JsonArray>(values)) {}
Json::Json(const Json::object& values) : m_ptr(std::make_shared<JsonObject>(values)) {}

Json::Type Json::type()                           const { return m_ptr->type(); }
double Json::number_value()                       const { return m_ptr->number_value(); }
int Json::int_value()                             const { return m_ptr->int_value(); }
bool Json::bool_value()                           const { return m_ptr->bool_value(); }
const std::string& Json::string_value()               const { return m_ptr->string_value(); }
const std::vector<Json>& Json::array_items()          const { return m_ptr->array_items(); }
const std::unordered_map<std::string, Json>& Json::object_items()    const { return m_ptr->object_items(); }
const Json& Json::operator[] (size_t i)          const { return (*m_ptr)[i]; }
const Json& Json::operator[] (const std::string& key) const { return (*m_ptr)[key]; }


Json Json::parse(const std::string& in)
{

    JsonParser parser{ in };
    Json result = parser.parse_json();

    parser.consume_whitespace();
    if (parser.failed)
        return Json();
    if (parser.i != in.size())
    {
        parser.fail("error");
        return Json();
    }

    return result;
}

void Json::dump(std::string& out) const
{
    m_ptr->dump(out);
}
std::string Json::dump() const
{
    std::string out;
    dump(out);
    return out;
}

