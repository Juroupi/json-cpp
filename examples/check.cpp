#include <iostream>
#include <fstream>
#include <json/utils.h>

int main() {

    auto input = std::ifstream("example.json");

    if (JSON::check(input)) {
        std::cout << "Valid JSON" << std::endl;
    } else {
        std::cout << "Invalid JSON" << std::endl;
    }

    return 0;
}