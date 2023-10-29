#include <iostream>
#include <fstream>
#include <json/parser.h>

class TestParser : public JSON::Parser {

    int tabs = 0;

    void printTabs() {
        for (int i = 0; i < tabs; i++) {
            std::cout << "    ";
        }
    }

    void onObjectStart() override {
        printTabs();
        tabs++;
        std::cout << "object start" << std::endl;
    }

    void onObjectEnd() override {
        tabs--;
        printTabs();
        std::cout << "object end" << std::endl;
    }

    void onArrayStart() override {
        printTabs();
        tabs++;
        std::cout << "array start" << std::endl;
    }

    void onArrayEnd() override {
        tabs--;
        printTabs();
        std::cout << "array end" << std::endl;
    }

    void onKey(std::string& key) override {
        printTabs();
        std::cout << "key: " << key << std::endl;
    }

    void onNumber(double value) override {
        printTabs();
        std::cout << "number: " << value << std::endl;
    }

    void onBoolean(bool value) override {
        printTabs();
        std::cout << "boolean: " << (value ? "true" : "false") << std::endl;
    }

    void onString(std::string& value) override {
        printTabs();
        std::cout << "string: " << value << std::endl;
    }

    void onNull() override {
        printTabs();
        std::cout << "null" << std::endl;
    }
};

int main() {

    auto input = std::ifstream("example.json");

    TestParser parser;

    parser.parse(input);

    return 0;
}