#include <iostream>
#include <fstream>
#include <json/value.h>

int main() {

    auto input = std::ifstream("example.json");

    try {
        JSON::Value value;
        input >> value;
        std::cout << value << std::endl;
    } catch (const JSON::Error& error) {
        std::cout << std::endl << "Error: " << error << std::endl;
    }

    return 0;
}