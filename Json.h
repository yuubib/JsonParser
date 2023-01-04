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
    Json(int value);                // NUMBER
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
    int int_value() const;
    bool bool_value() const;
    const std::string& string_value() const;

    const array& array_items() const;
    const object& object_items() const;


    const Json& operator[](size_t i) const;
  
    const Json& operator[](const std::string& key) const;

    static Json parse(const std::string& in);
    void dump(std::string& out) const;
    std::string dump() const;
    std::string pretty_print() const;

};
inline bool in_range(long x, long lower, long upper)
{
    return (x >= lower && x <= upper);
}
