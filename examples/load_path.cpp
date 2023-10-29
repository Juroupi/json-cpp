#include <iostream>
#include <fstream>
#include <json/value.h>

static std::ifstream input;

void printValue(const JSON::Path& path, bool unique) {

    JSON::Value value;

    value.parse(input, path, unique);

    std::cout << path << ": ";
    if (!value.isUndefined()) {
        std::cout << value;
    }
    std::cout << std::endl;

    input.seekg(0);
}

int main() {

    input.open("example.json");

    try {

        JSON::Value value;

        printValue("$['true'][1]", true);
        printValue("$['true'][1]", false);

        printValue("$['null']['a']", true);
        printValue("$['null']['a']", false);

        printValue("$['null']['c']", false);

        printValue("$['?']", true);
        printValue("$['?']", false);

        printValue("$['salut', 'value']", false);

        printValue("$['numbers'][1:5:2]", false);

        printValue("$[*]['c']", false);
        printValue("$.['c']", false);

        printValue("$.['b'].['d']", false);

        printValue("$", true);

    } catch (const JSON::Error& error) {
        std::cout << std::endl << "Error: " << error << std::endl;
    }

    return 0;
}