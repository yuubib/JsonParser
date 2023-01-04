#include <string>
#include <iostream>
#include "Json.h"

int main()
{
    
    auto str = R"({"id": "0001", "type": "donut","name": "Cake","ppu": 0.55, "batters":{"batter":[{ "id": "1001", "type": "Regular" },{ "id": "1002", "type": "Chocolate" }]},"topping":[{ "id": "5001", "type": "None" },{ "id": "5002", "type": "Glazed" }]})";
    Json res = Json::parse(str);
    std::cout << "pertty_print:" << std::endl;
    std::cout << res.pretty_print() << std::endl;
    std::cout << "normal:" << std::endl;
    std::cout << res.dump() << std::endl;
    return 0;
}