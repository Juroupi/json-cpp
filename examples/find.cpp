#include <iostream>
#include <fstream>
#include <json/value.h>

static JSON::Value value;

void printValue(const JSON::Path& path) {

    auto children = value.findAll(path);

    std::cout << path << ": [";
    
    for (size_t i = 0; i < children.size(); ++i) {
        if (i > 0) {
            std::cout << ",";
        }
        std::cout << *children[i];
    }

    std::cout << "]" << std::endl;
}

int main() {

    value = JSON::load("example.json");

    printValue("$['true'][1]");

    printValue("$['null']['a']");

    printValue("$['null']['c']");

    printValue("$['?']");

    printValue("$['salut', 'value']");

    printValue("$['numbers'][1:5:2]");

    printValue("$[*]['c']");

    printValue("$.['c']");

    printValue("$.['b'].['d']");

    printValue("$");

    return 0;
}