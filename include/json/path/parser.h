#ifndef _JSON_PATH_PARSER_H_
#define _JSON_PATH_PARSER_H_

#include <json/path.h>
#include <json/path/lexer.h>
#include <string>
#include <memory>

namespace JSON {

class Path::Parser {

    Lexer lexer;
    Path& path;
    bool deepScan;

    std::unique_ptr<Path::Element> parseUnique();

    std::unique_ptr<Path::Element> parseOptions();

    std::unique_ptr<Path::Element> parseElement();

public:

    Parser(Path& path, const std::string& input);

    void parse();
};

}

#endif