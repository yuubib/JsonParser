#pragma once

#include "Json.h"
#include <string>
#include <sstream>
#include <cmath>
class JsonValue 
{
public:
    Json::Type tag;
    Json::Type type() { return tag; };
    virtual void dump(std::string& out) const = 0;
    virtual double number_value() const { return 0; };
    virtual int int_value() const { return 0; };
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
    virtual void dump(std::string& out) const override
    {
        out += "null";
    }

};

class JsonDouble :public JsonValue
{
public:
    double m_value;
    JsonDouble(double& value) :m_value(value) { tag = Json::NUMBER; }
    virtual void dump(std::string& out) const override
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
    virtual double number_value() const override { return m_value; }
    virtual int int_value() const override { return static_cast<int>(m_value); }
};

class JsonInt :public JsonValue
{
public:
    int m_value;
    JsonInt(int& value) :m_value(value) { tag = Json::NUMBER; }
    virtual void dump(std::string& out) const override
    {
        char buf[32];
        snprintf(buf, sizeof buf, "%d", m_value);
        out += buf;
    }
    virtual double number_value() const override { return m_value; }
    virtual int int_value() const override { return m_value; }
};

class JsonBoolean :public JsonValue
{
public:
    bool m_value;
    JsonBoolean(bool& value) :m_value(value) { tag = Json::BOOL; }
    virtual void dump(std::string& out) const override
    {
        out += m_value ? "ture" : "false";
    }
    virtual bool bool_value() const override { return m_value; }
};

class JsonString :public JsonValue
{
public:
    std::string m_value;
    JsonString(const std::string& value) :m_value(value) { tag = Json::STRING; }
    virtual const std::string& string_value()const override { return m_value; };
    virtual void dump(std::string& out) const override
    {
        std::stringstream ss;
        ss << "\"" << m_value << "\"";
        out += ss.str();
    }
};

class JsonArray :public JsonValue
{
public:
    Json::array m_value;
    JsonArray(const Json::array& value) :m_value(value) { tag = Json::ARRAY; }
    virtual void dump(std::string& out) const override
    {
        bool first = true;
        out += "[";
        for (const auto& value : m_value)
        {
            if (!first)
                out += ", ";
            value.dump(out);
            first = false;
        }
        out += "]";
    }
    const Json::array& array_items() const override { return m_value; }
    const Json& operator[](size_t i) const override { return m_value[i]; }
};

class JsonObject :public JsonValue
{
public:
    Json::object m_value;
    JsonObject(const Json::object& value) :m_value(value) { tag = Json::OBJECT; }
    virtual void dump(std::string& out) const override
    {
        bool first = true;
        out += "{";
        for (const auto& kv : m_value)
        {
            if (!first)
                out += ", ";
            out += kv.first;
            out += ": ";
            kv.second.dump(out);
            first = false;
        }
        out += "}";
    }
    const Json::object& object_items() const override { return m_value; }
    const Json& operator[](const std::string& key) const override
    {
        auto iter = m_value.find(key);
        return iter->second;
    }
};
