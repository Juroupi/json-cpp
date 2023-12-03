#include <iostream>
#include <fstream>
#include <json/lexer.h>

int main() {

    auto stream = std::ifstream("example.json");

    JSON::Lexer lexer(stream);

    while (lexer.getToken() != JSON::Token::END_OF_STREAM) {
        std::cout << lexer.getToken() << " ";
        lexer.nextToken();
    }

    std::cout << std::endl;

    return 0;
}