#include <iostream>
#include <fstream>
#include <json/lexer.h>

int main() {

    auto stream = std::ifstream("example.json");

    JSON::Lexer lexer(stream);

    JSON::Token token;

    while ((token = lexer.getNextToken()) != JSON::Token::END_OF_STREAM) {
        std::cout << token << " ";
    }

    std::cout << std::endl;

    return 0;
}