#ifndef _JSON_PARSER_H_
#define _JSON_PARSER_H_

#include <json/lexer.h>
#include <json/error.h>
#include <json/path.h>
#include <string>
#include <istream>

namespace JSON {

/**
 * A JSON parser.
 * This class is used to parse data and call the appropriate callbacks
 * in the order that the tokens are read from the input.
 */
class Parser {
    
    void expectToken(Lexer& lexer, Token expectedToken);

    void parseValue(Lexer& lexer, Path::Cursor& cursor);
    void parseValue(Lexer& lexer, Path::Cursor& cursor, Token token);
    void parseObject(Lexer& lexer, Path::Cursor& cursor);
    void parseNonEmptyObject(Lexer& lexer, Path::Cursor& cursor);
    void parseArray(Lexer& lexer, Path::Cursor& cursor);
    void parseNonEmptyArray(Lexer& lexer, Path::Cursor& cursor, size_t index = 1);

public:

    /**
     * An error that occurred during parsing.
     */
    struct Error : public JSON::Error {

        /**
         * The token that caused the error.
         */
        Token token;

        /**
         * The line and character position of the token in the input.
         */
        int linePos, charPos;

        Error(Token token, const Lexer& lexer);
    };

    /**
     * Parse the given input stream.
     * The callbacks are called only for the content that is in the given path.
     */
    void parse(std::istream& input, const Path& path = {});

    /**
     * Callbacks that are called during parsing.
     */
    virtual void onObjectStart() = 0;
    virtual void onObjectEnd() = 0;
    virtual void onArrayStart() = 0;
    virtual void onArrayEnd() = 0;
    virtual void onKey(std::string& key) = 0;
    virtual void onNumber(double number) = 0;
    virtual void onBoolean(bool boolean) = 0;
    virtual void onString(std::string& string) = 0;
    virtual void onNull() = 0;
};

}

#endif