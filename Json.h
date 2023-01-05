#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <cmath>
class JsonValue;
class Json
{
public:
    enum Type {
        NUL, 
        NUMBER, 
        BOOL, 
        STRING, 
        ARRAY, 
        OBJECT
    };
    using array = std::vector<Json>;
    using object = std::unordered_map < std::string, Json >;
    std::shared_ptr<JsonValue> m_ptr;

    Json();
    Json(double value);             // NUMBER
    Json(long long value);                // NUMBER
    Json(bool value);               // BOOL
    Json(const std::string& value); // STRING   
    Json(const char* value);       // STRING
    Json(const array& values);      // ARRAY
    Json(const object& values);     // OBJECT

    Type type() const;
    bool is_null()   const { return type() == NULL; }
    bool is_number() const { return type() == NUMBER; }
    bool is_bool()   const { return type() == BOOL; }
    bool is_string() const { return type() == STRING; }
    bool is_array()  const { return type() == ARRAY; }
    bool is_object() const { return type() == OBJECT; }

    double number_value() const;
    long long int_value() const;
    bool bool_value() const;
    const std::string& string_value() const;

    const array& array_items() const;
    const object& object_items() const;


    const Json& operator[](size_t i) const;
  
    const Json& operator[](const std::string& key) const;

    static Json parse(const std::string& in);
    void dump(std::string& out,int depth = -1) const;
    std::string dump(int depth = -1) const;
   // std::string pretty_print() const;

};
inline bool in_range(long long x, long long lower, long long upper)
{
    return (x >= lower && x <= upper);
}
inline void encode_utf8(long pt, std::string& out) {
    if (pt < 0)
        return;

    if (pt < 0x80) {
        out += static_cast<char>(pt);
    }
    else if (pt < 0x800) {
        out += static_cast<char>((pt >> 6) | 0xC0);
        out += static_cast<char>((pt & 0x3F) | 0x80);
    }
    else if (pt < 0x10000) {
        out += static_cast<char>((pt >> 12) | 0xE0);
        out += static_cast<char>(((pt >> 6) & 0x3F) | 0x80);
        out += static_cast<char>((pt & 0x3F) | 0x80);
    }
    else {
        out += static_cast<char>((pt >> 18) | 0xF0);
        out += static_cast<char>(((pt >> 12) & 0x3F) | 0x80);
        out += static_cast<char>(((pt >> 6) & 0x3F) | 0x80);
        out += static_cast<char>((pt & 0x3F) | 0x80);
    }
}
