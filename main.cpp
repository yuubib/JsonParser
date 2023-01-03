#include <string>
#include <iostream>
#include "Json.h"

int main()
{
    auto str = R"(
    {
      "name": "ZYB",
      "university": "º¼µç",
      "age": 22,
      "isMale": true,
      "PI":3.14,
      "Array":[1,true,"12\"34\t56"]
    }
  )";
    Json res = Json::parse(str);
    std::cout << res.dump() << std::endl;
    std::unordered_map<int, std::string> type{
        {Json::NUL,"NULL"},
        {Json::NUMBER,"NUMBER"},
        {Json::BOOL,"BOOL"},
        {Json::STRING,"STRING"},
        {Json::ARRAY,"ARRAY"},
        {Json::OBJECT,"OBJECT"}
    };
    for (auto i : res.object_items())
    {
        std::cout << i.first.c_str() << ":" << type[i.second.type()] << std::endl;
    }
    return 0;
}