#include <iostream>
#include <sstream>
#include <json/parser.h>

class BParser : public JSON::Parser {
    void onObjectStart() override {}
    void onObjectEnd() override {}
    void onArrayStart() override {}
    void onArrayEnd() override {}
    void onKey(std::string& key) override {}
    void onIndex(size_t index) override {}
    void onNumber(double value) override {
        std::cout << "b: " << value << std::endl;
    }
    void onBoolean(bool value) override {}
    void onString(std::string& value) override {}
    void onNull() override {}
};

class AParser : public JSON::Parser {
    void onObjectStart() override {}
    void onObjectEnd() override {}
    void onArrayStart() override {}
    void onArrayEnd() override {}
    void onKey(std::string& key) override {
        if (key == "sub") {
            BParser bParser;
            delegate(bParser);
        }
    }
    void onIndex(size_t index) override {}
    void onNumber(double value) override {
        std::cout << "a: " << value << std::endl;
    }
    void onBoolean(bool value) override {}
    void onString(std::string& value) override {}
    void onNull() override {}
};

int main() {

    std::string str = R"({
        "a1": 12,
        "sub": {
            "b1": 34,
            "b2": 56
        },
        "a2": 78
    })";

    auto input = std::istringstream(str);

    AParser aParser;

    aParser.parse(input);

    return 0;
}