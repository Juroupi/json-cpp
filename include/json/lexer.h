#ifndef _JSON_LEXER_H_
#define _JSON_LEXER_H_

#include <istream>
#include <ostream>
#include <string>
#include <json/error.h>

namespace JSON {

/**
 * The different types of tokens that can be read.
 */
enum class Token {
    OBJECT_START,
    OBJECT_END,
    ARRAY_START,
    ARRAY_END,
    COLON,
    COMMA,
    NUMBER,
    STRING,
    BOOLEAN,
    NULL_,
    END_OF_STREAM
};

/**
 * A JSON lexer.
 */
class Lexer {

public:

    /**
     * An error that occurred during lexing.
     */
    struct Error : public JSON::Error {

        /**
         * The different types of lexing errors.
         */
        enum Code {
            INVALID_CHARACTER,              // An invalid character was encountered.
            INVALID_NUMBER,                 // A number was not correctly formatted.
            UNTERMINATED_STRING,            // A string was not terminated before the end of the stream.
            INVALID_ESCAPE_SEQUENCE,        // An invalid escape sequence was encountered.
            INVALID_UNICODE_ESCAPE_SEQUENCE // An invalid unicode escape sequence was encountered.
        };

        /**
         * The type of lexing error.
         */
        Code code;

        /**
         * The character that caused the error.
         */
        char c;

        /**
         * The position of the error.
         */
        int lineNumber, charPos;

        Error(Code code, const Lexer& lexer, char c = '\0');
    };

private:

    std::istream& input;
    int charPos = 0, lineNumber = 1;
    int tokenCharPos = charPos, tokenLineNumber = lineNumber;
    bool carriageReturn = false;

    double numberValue;
    std::string stringValue;
    bool booleanValue;

    void nextLine();
    char getNextChar();
  
    void getNextNumber(char c);
    void getNextString();

public:

    /**
     * Construct a new Lexer object that reads from the given input stream.
     */
    Lexer(std::istream& input);
    
    /**
     * Get the next token from the input stream.
     */
    Token getNextToken();

    /**
     * Get the position of the last character read in the current line.
     */
    int getCharPos() const;

    /**
     * Get the current line number.
     */
    int getLineNumber() const;

    /**
     * Get the position of the first character of last token read in the current line.
     */
    int getTokenCharPos() const;

    /**
     * Get the line number of the last token read.
     */
    int getTokenLineNumber() const;
    
    /**
     * Get the value of the last number read.
     */
    double getNumberValue() const;

    /**
     * Get the value of the last string read.
     */
    const std::string& getStringValue() const;

    /**
     * Get the value of the last string read.
     * This version allows to use std::move on the returned string to avoid copying.
     */
    std::string& getStringValue();

    /**
     * Get the value of the last boolean read.
     */
    bool getBooleanValue() const;
};

}

/**
 * Print the name of a token to a stream.
 */
std::ostream& operator<<(std::ostream& stream, JSON::Token token);

#endif