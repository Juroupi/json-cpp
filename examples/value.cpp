#include <iostream>
#include <json/value.h>

int main() {
    
    JSON::Value e = JSON::Object {
        { "a", JSON::String { "test" } },
        { "b", JSON::Array { 1.0, true, JSON::null } }
    };

    std::cout << e << std::endl;

    std::swap(e["a"], e["b"][0]);

    std::cout << e << std::endl;

    e["a"] = JSON::String { "a" };
    e["b"].setStringValue("b");

    std::cout << e << std::endl;

    e = std::move(e);

    std::cout << e << std::endl;

    std::cout << (JSON::Value(12.0) == 11.0) << std::endl;

    return 0;
}