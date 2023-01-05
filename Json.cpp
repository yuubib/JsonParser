#include "Json.h"
#include "JsonValue.h"
#include "parser.h"
#include <memory>
#include <stack>
Json::Json() :m_ptr(std::make_shared<JsonNull>()) {}
Json::Json(double value) :m_ptr(std::make_shared<JsonDouble>(value)) {}
Json::Json(long long value) :m_ptr(std::make_shared<JsonInt>(value)) {}
Json::Json(bool value) :m_ptr(std::make_shared<JsonBoolean>(value)) {}
Json::Json(const std::string& value) : m_ptr(std::make_shared<JsonString>(value)) {}
Json::Json(const char* value) : m_ptr(std::make_shared<JsonString>(value)) {}
Json::Json(const Json::array& values) : m_ptr(std::make_shared<JsonArray>(values)) {}
Json::Json(const Json::object& values) : m_ptr(std::make_shared<JsonObject>(values)) {}

Json::Type Json::type()                           const { return m_ptr->type(); }
double Json::number_value()                       const { return m_ptr->number_value(); }
long long Json::int_value()                             const { return m_ptr->int_value(); }
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

void Json::dump(std::string& out,int depth) const
{
    m_ptr->dump(out, depth);
}
std::string Json::dump(int depth) const
{
    std::string out;
    dump(out,depth);
    return out;
}
/*
std::string Json::pretty_print() const
{
    std::string in = dump();
    struct outchar
    {
        char value;
        int depth;
    };
    std::stack<outchar> stk;
    std::string out;
    for (int i = 0; i < in.size(); i++)
    {
        if (in[i] == '{' or in[i] == '[')
        {
            if (stk.empty())
            {
                stk.push({ in[i],0 });
            }
            else
                stk.push({ in[i],stk.top().depth + 1 });
            if (!(i - 1 < 0 or in[i - 1] == ',' or in[i - 1] == '{' or in[i - 1] == '['))
            {
                out.push_back('\n');
                out.append(stk.top().depth * 4, ' ');
            }
            out.push_back(in[i]);
            out.push_back('\n');
            out.append((stk.top().depth + 1) * 4, ' ');
        }
        else if (in[i] == ',')
        {
            out.push_back(',');
            out.push_back('\n');
            out.append((stk.top().depth + 1) * 4, ' ');
            
        }
        else if (in[i] == '}' or in[i] == ']')
        {
            out.push_back('\n');
            out.append(stk.top().depth * 4, ' ');
            out.push_back(in[i]);
            stk.pop();
        }
        else if (in[i] == '"')
        {
            out.push_back(in[i]);
            i++;
            while (!(in[i]=='"' and in[i-1]!='\''))
            {
                i++;
                out.push_back(in[i]);
            }
        }
    }
    return out;
}
*/