#ifndef _JSON_PATH_LEXER_H_
#define _JSON_PATH_LEXER_H_

#include <json/path.h>
#include <string>
#include <sstream>

namespace JSON {

enum class Path::Token {
    DOLLAR,
    STRING,
    NUMBER,
    STAR,
    DOT,
    LBRACKET,
    RBRACKET,
    COLON,
    COMMA,
    END
};

struct Path::Lexer {

    std::istringstream input;
    std::string stringValue;
    size_t numberValue;
    Token token;

    Lexer(const std::string& path);

    Token getNextToken();
};

}

#endif