#include <json/path/parser.h>
#include <json/path/element.h>
#include <exception>

namespace JSON {

Path::Parser::Parser(Path& path, const std::string& input) :
        lexer(input), path(path) {}

std::unique_ptr<Path::Element> Path::Parser::parseUnique() {

    if (lexer.token == Token::STAR) {
        lexer.getNextToken();
        return std::unique_ptr<Path::Element::Any>(new Path::Element::Any());
    }

    if (lexer.token == Token::STRING) {
        lexer.getNextToken();
        return std::unique_ptr<Path::Element::Name>(new Path::Element::Name(std::move((Path::Element::Name&)lexer.stringValue)));
    }

    if (lexer.token == Token::NUMBER) {
        size_t first = lexer.numberValue;
        size_t last = lexer.numberValue;
        size_t step = 1;
        if (lexer.getNextToken() == Token::COLON) {
            if (lexer.getNextToken() == Token::NUMBER) {
                last = lexer.numberValue;
                if (lexer.getNextToken() == Token::COLON) {
                    if (lexer.getNextToken() == Token::NUMBER) {
                        step = lexer.numberValue;
                        lexer.getNextToken();
                    } else {
                        throw std::runtime_error("Expected number after ':' in path");
                    }
                }
            } else {
                throw std::runtime_error("Expected number after ':' in path");
            }
        }
        if (first == last) {
            return std::unique_ptr<Path::Element::Index>(new Path::Element::Index(first));
        }
        return std::unique_ptr<Path::Element::Range>(new Path::Element::Range(first, last, step));
    }

    throw std::runtime_error("Unexpected token in path");
}

std::unique_ptr<Path::Element> Path::Parser::parseOptions() {

    std::unique_ptr<Path::Element> element = parseUnique();

    if (lexer.token == Token::COMMA) {
        lexer.getNextToken();
        std::unique_ptr<Path::Element> second = parseOptions();
        return std::unique_ptr<Path::Element::Options>(new Path::Element::Options(std::move(element), std::move(second)));
    }

    return element;
}

std::unique_ptr<Path::Element> Path::Parser::parseElement() {

    if (lexer.token == Token::DOT) {
        deepScan = true;
        lexer.getNextToken();
    } else {
        deepScan = false;
    }

    if (lexer.token == Token::LBRACKET) {
        lexer.getNextToken();
        std::unique_ptr<Path::Element> element = parseOptions();
        if (lexer.token == Token::RBRACKET) {
            lexer.getNextToken();
            return element;
        }
        throw std::runtime_error("Expected ']' in path");
    }

    throw std::runtime_error("Expected '[' in path");
}

void Path::Parser::parse() {
    if (lexer.getNextToken() == Token::DOLLAR) {
        lexer.getNextToken();
    }
    while (lexer.token != Token::END) {
        std::unique_ptr<Path::Element> element = parseElement();
        if (deepScan) {
            path << DeepScan();
        }
        path << std::move(element);
    }
}

}