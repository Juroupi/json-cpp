#include <JSON/parser.h>
#include <JSON/lexer.h>
#include <JSON/path/cursor.h>
#include <sstream>

namespace JSON {

void Parser::parse(std::istream& input, const Path& path) {
    Lexer lexer(input);
    Path::Cursor cursor(path);
    parseValue(lexer, cursor);
}

Parser::Error::Error(Token token, const Lexer& lexer) :
    token(token), linePos(lexer.getTokenLineNumber()), charPos(lexer.getTokenCharPos()) {
    std::ostringstream s;
    s << "unexpected token " << token << " at line " << linePos << " (char " << charPos << ")";
    message = s.str();
}

void Parser::expectToken(Lexer& lexer, Token expectedToken) {
    Token token = lexer.getNextToken();
    if (token != expectedToken) {
        throw Error(token, lexer);
    }
}

void Parser::parseValue(Lexer& lexer, Path::Cursor& cursor) {
    parseValue(lexer, cursor, lexer.getNextToken());
}

void Parser::parseValue(Lexer& lexer, Path::Cursor& cursor, Token token) {
    
    switch (token) {

        case Token::OBJECT_START:
            if (cursor.isInTarget()) onObjectStart();
            parseObject(lexer, cursor);
            if (cursor.isInTarget()) onObjectEnd();
            break;
        
        case Token::ARRAY_START:
            if (cursor.isInTarget()) onArrayStart();
            parseArray(lexer, cursor);
            if (cursor.isInTarget()) onArrayEnd();
            break;
        
        case Token::NUMBER:
            if (cursor.isInTarget()) onNumber(lexer.getNumberValue());
            break;

        case Token::BOOLEAN:
            if (cursor.isInTarget()) onBoolean(lexer.getBooleanValue());
            break;
        
        case Token::STRING:
            if (cursor.isInTarget()) onString(lexer.getStringValue());
            break;

        case Token::NULL_:
            if (cursor.isInTarget()) onNull();
            break;
        
        default:
            throw Error(token, lexer);
    }
}

void Parser::parseObject(Lexer& lexer, Path::Cursor& cursor) {

    Token token = lexer.getNextToken();
    
    switch (token) {

        case Token::OBJECT_END:
            break;
        
        case Token::STRING:
            if (cursor.isInTarget()) {
                cursor.next(lexer.getStringValue());
                onKey(lexer.getStringValue());
            } else {
                cursor.next(lexer.getStringValue());
            }
            expectToken(lexer, Token::COLON);
            parseValue(lexer, cursor);
            cursor.prev();
            return parseNonEmptyObject(lexer, cursor);

        default:
            throw Error(token, lexer);
    }
}

void Parser::parseNonEmptyObject(Lexer& lexer, Path::Cursor& cursor) {

    Token token = lexer.getNextToken();
    
    switch (token) {

        case Token::OBJECT_END:
            break;
        
        case Token::COMMA:
            expectToken(lexer, Token::STRING);
            if (cursor.isInTarget()) {
                cursor.next(lexer.getStringValue());
                onKey(lexer.getStringValue());
            } else {
                cursor.next(lexer.getStringValue());
            }
            expectToken(lexer, Token::COLON);
            parseValue(lexer, cursor);
            cursor.prev();
            return parseNonEmptyObject(lexer, cursor);

        default:
            throw Error(token, lexer);
    }
}

void Parser::parseArray(Lexer& lexer, Path::Cursor& cursor) {

    Token token = lexer.getNextToken();

    switch (token) {

        case Token::ARRAY_END:
            break;

        default:
            cursor.next(0);
            parseValue(lexer, cursor, token);
            cursor.prev();
            return parseNonEmptyArray(lexer, cursor);
    }
}

void Parser::parseNonEmptyArray(Lexer& lexer, Path::Cursor& cursor, size_t index) {

    Token token = lexer.getNextToken();
    
    switch (token) {

        case Token::ARRAY_END:
            break;
        
        case Token::COMMA:
            cursor.next(index);
            parseValue(lexer, cursor);
            cursor.prev();
            return parseNonEmptyArray(lexer, cursor, index + 1);

        default:
            throw Error(token, lexer);
    }
}

}