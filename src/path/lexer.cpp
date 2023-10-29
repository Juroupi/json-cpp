#include <json/path/lexer.h>
#include <exception>

namespace JSON {

Path::Lexer::Lexer(const std::string& path) :
    input(path) {}

Path::Token Path::Lexer::getNextToken() {

    int c = input.get();

    if (c == ' ') return getNextToken();
    if (c == '.') return token = Token::DOT;
    if (c == '*') return token = Token::STAR;
    if (c == ',') return token = Token::COMMA;
    if (c == ':') return token = Token::COLON;
    if (c == '$') return token = Token::DOLLAR;
    if (c == '[') return token = Token::LBRACKET;
    if (c == ']') return token = Token::RBRACKET;
    if (c == EOF) return token = Token::END;

    if (c == '\"' || c == '\'') {
        char end = c;
        stringValue.clear();
        while (c = input.get(), c != end) {
            if (c == EOF) throw std::runtime_error("Unexpected end of string in path");
            stringValue += c;
        }
        return token = Token::STRING;
    }

    if (c > 'a' && c < 'z' || c > 'A' && c < 'Z' || c == '_') {
        stringValue.clear();
        stringValue += c;
        while (c = input.get(), c > 'a' && c < 'z' || c > 'A' && c < 'Z' || c > '0' && c < '9' || c == '_') {
            stringValue += c;
        }
        return token = Token::STRING;
    }

    if (c >= '0' && c <= '9') {
        input.unget();
        input >> numberValue;
        return token = Token::NUMBER;
    }

    throw std::runtime_error("Unexpected character in path");
}

}