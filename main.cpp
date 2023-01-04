#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Json.h"
int main()
{
    /*
    std::ifstream f(R"(C:\Users\Administrator\Desktop\citm_catalog.json)");
    std::stringstream ss;
    ss << f.rdbuf();

    auto str = ss.str();
    */

    auto str = R"({"id": "0001", "type": "donut","name": "Cake","ppu": 0.55, "batters":{"batter":[{ "id": "1001", "type": "Regular" },{ "id": "1002", "type": "Chocolate" }]},"topping":[{ "id": "5001", "type": "None" },{ "id": "5002", "type": "Glazed" }]})";
    Json res = Json::parse(str);

    std::cout << "pertty_print:" << std::endl;
    std::cout << res.dump(0) << std::endl;//参数0是pretty_print

    std::cout << "normal:" << std::endl;
    std::cout << res.dump() << std::endl;//空参数是普通

    return 0;
}