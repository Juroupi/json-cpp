#include <iostream>
#include <fstream>
#include <json/error.h>
#include <json/utils.h>

int main() {

    auto input = std::ifstream("example.json");

    try {
        JSON::copy(std::cout, input, 2, true);
        std::cout << std::endl;
    } catch (const JSON::Error& error) {
        std::cout << std::endl << "Error: " << error << std::endl;
    }

    return 0;
}