#pragma once

#include "Json.h"
#include <string>
#include<sstream>
class JsonValue 
{
public:
    Json::Type tag;
    Json::Type type() { return tag; };

    virtual void dump(std::string& out,int depth = -1) const = 0;

    virtual double number_value() const { return 0; };
    virtual long long int_value() const { return 0; };
    virtual bool bool_value() const { return false; };
    virtual const std::string& string_value() const { return ""; };
    virtual const Json::array& array_items() const { return Json::array(); };
    virtual const Json& operator[](size_t i) const { return Json(); };
    virtual const Json::object& object_items() const { return Json::object(); };
    virtual const Json& operator[](const std::string& key) const { return Json(); };
    virtual ~JsonValue() {}
};

class JsonNull :public JsonValue
{
public:
    JsonNull() { tag = Json::NUL; }

    virtual void dump(std::string& out, int depth) const override
    {
        if (depth <= -1)
            out += "null";
        else
        {
            out.append(depth * 4, ' ');
            out += "null";
        }
    }
};

class JsonDouble :public JsonValue
{
public:
    double m_value;
    JsonDouble(double& value) :m_value(value) { tag = Json::NUMBER; }

    virtual void dump(std::string& out,int depth) const override
    {
        if (depth <= -1)
        {
            if (std::isfinite(m_value)) {
                char buf[32];
                snprintf(buf, sizeof buf, "%.17g", m_value);
                out += buf;
            }
            else {
                out += "null";
            }
        }
        else
        {
            if (std::isfinite(m_value)) {
                char buf[32];
                snprintf(buf, sizeof buf, "%.17g", m_value);
                out.append(depth * 4, ' ');
                out += buf;
            }
            else {
                out.append(depth * 4, ' ');
                out += "null";
            }
        }
    }

    virtual double number_value() const override { return m_value; }
    virtual long long int_value() const override { return static_cast<int>(m_value); }
};

class JsonInt :public JsonValue
{
public:
    long long m_value;
    JsonInt(long long& value) :m_value(value) { tag = Json::NUMBER; }

    virtual void dump(std::string& out, int depth) const override
    {
        char buf[32];
        snprintf(buf, sizeof buf, "%ld", m_value);
        if (depth <= -1)
            out += buf;
        else
        {
            out.append(depth * 4, ' ');
            out += buf;
        }
    }

    virtual double number_value() const override { return m_value; }
    virtual long long int_value() const override { return m_value; }
};

class JsonBoolean :public JsonValue
{
public:
    bool m_value;
    JsonBoolean(bool& value) :m_value(value) { tag = Json::BOOL; }
    virtual void dump(std::string& out, int depth) const override
    {
        if (depth <= -1)
            out += m_value ? "true" : "false";
        else
        {
            out.append(depth * 4, ' ');
            out += m_value ? "true" : "false";
        }
    }
    virtual bool bool_value() const override { return m_value; }
};

class JsonString :public JsonValue
{
public:
    std::string m_value;
    JsonString(const std::string& value) :m_value(value) { tag = Json::STRING; }
    virtual const std::string& string_value()const override { return m_value; };
    virtual void dump(std::string& out, int depth) const override
    {
        if (depth >= 0)
        {
            out.append(depth * 4, ' ');
        }
        out += '\"';
        for (size_t i = 0; i < m_value.length(); i++)
        {
            const char ch = m_value[i];
            if (ch == '\\') {
                out += "\\\\";
            }
            else if (ch == '"') {
                out += "\\\"";
            }
            else if (ch == '\b') {
                out += "\\b";
            }
            else if (ch == '\f') {
                out += "\\f";
            }
            else if (ch == '\n') {
                out += "\\n";
            }
            else if (ch == '\r') {
                out += "\\r";
            }
            else if (ch == '\t') {
                out += "\\t";
            }
            else if (static_cast<uint8_t>(ch) <= 0x1f) {
                char buf[8];
                snprintf(buf, sizeof buf, "\\u%04x", ch);
                out += buf;
            }
            else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(m_value[i + 1]) == 0x80
                && static_cast<uint8_t>(m_value[i + 2]) == 0xa8) {
                out += "\\u2028";
                i += 2;
            }
            else if (static_cast<uint8_t>(ch) == 0xe2 && static_cast<uint8_t>(m_value[i + 1]) == 0x80
                && static_cast<uint8_t>(m_value[i + 2]) == 0xa9) {
                out += "\\u2029";
                i += 2;
            }
            else {
                out += ch;
            }
        }
        out += '\"';
    }
};

class JsonArray :public JsonValue
{
public:
    Json::array m_value;
    JsonArray(const Json::array& value) :m_value(value) { tag = Json::ARRAY; }
    virtual void dump(std::string& out,int depth) const override
    {
        if (depth <= -1)
        {
            bool first = true;
            out += "[";
            for (const auto& value : m_value)
            {
                if (!first)
                    out += ",";
                value.dump(out);
                first = false;
            }
            out += "]";
        }
        else
        {
            bool first = true;
            out.append(depth * 4, ' ');
            out += "[\n";
            for (const auto& value : m_value)
            {
                if (!first)
                    out += ",\n";
                value.dump(out, depth + 1);
                first = false;
            }
            out += '\n';
            out.append(depth * 4, ' ');
            out += "]";
        }
    }
    const Json::array& array_items() const override { return m_value; }
    const Json& operator[](size_t i) const override { return m_value[i]; }
};

class JsonObject :public JsonValue
{
public:
    Json::object m_value;
    JsonObject(const Json::object& value) :m_value(value) { tag = Json::OBJECT; }
    virtual void dump(std::string& out, int depth) const override
    {
        if (depth <= -1)
        {
            bool first = true;
            out += "{";
            for (const auto& kv : m_value)
            {
                if (!first)
                    out += ",";
                Json(kv.first).dump(out);
                out += ": ";
                kv.second.dump(out);
                first = false;
            }
            out += "}";
        }
        else
        {
            bool first = true;
            out.append(depth * 4, ' ');
            out += "{\n";
            for (const auto& kv : m_value)
            {
                if (!first)
                    out += ",\n";

                Json(kv.first).dump(out, depth + 1);
                //if (kv.first == "slash")
                 //   int x = 1;
                out += ": ";

                if (kv.second.type() != Json::ARRAY and kv.second.type() != Json::OBJECT)
                    kv.second.dump(out, -1);
                else
                {
                    out += '\n';
                    kv.second.dump(out, depth + 1);
                }
                first = false;
            }
            out += '\n';
            out.append(depth * 4, ' ');
            out += "}";
        }
    }
    const Json::object& object_items() const override { return m_value; }
    const Json& operator[](const std::string& key) const override
    {
        auto iter = m_value.find(key);
        return iter->second;
    }
};