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

    Lexer lexer;
    bool delegated = false;
    size_t depth;
    
    void expectToken(Token expectedToken);

    void parseValue(Path::Cursor& cursor);
    void parseObject(Path::Cursor& cursor);
    void parseNonEmptyObject(Path::Cursor& cursor);
    void parseArray(Path::Cursor& cursor);
    void parseNonEmptyArray(Path::Cursor& cursor, size_t index = 1);

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

        Error(const Lexer& lexer);
    };

    /**
     * Get the current depth of the parser.
     * It is incremented after every call to onObjectStart or onArrayStart and decremented before onObjectEnd or onArrayEnd.
     */
    size_t getDepth() const;

    /**
     * Parse the given input stream.
     * The callbacks are called only for the content that is in the given path.
     * This function must not be called while parsing is in progress (i.e. from a callback) in this parser instance.
     */
    void parse(std::istream& input, const Path& path = {});

    /**
     * Delegate the parsing of the incoming value to the given parser.
     * This can be used to parse a sub-object in a different way than the parent object.
     * This function can only be called from the callback onKey or onIndex of this parser.
     * This function must not be called while parsing is in progress in the given parser instance.
     */
    void delegate(Parser& parser, const Path& path = {});

    /**
     * Callbacks that are called during parsing.
     */
    virtual void onObjectStart() = 0;
    virtual void onObjectEnd() = 0;
    virtual void onArrayStart() = 0;
    virtual void onArrayEnd() = 0;
    virtual void onKey(std::string& key) = 0;
    virtual void onIndex(size_t index) = 0;
    virtual void onNumber(double number) = 0;
    virtual void onBoolean(bool boolean) = 0;
    virtual void onString(std::string& string) = 0;
    virtual void onNull() = 0;
};

}

#endif