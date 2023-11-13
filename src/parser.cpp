#include <json/parser.h>
#include <json/lexer.h>
#include <json/path/cursor.h>
#include <sstream>

namespace JSON {

size_t Parser::getDepth() const {
    return depth;
}

void Parser::parse(std::istream& input, const Path& path) {
    depth = 0;
    lexer.setInput(input);
    Path::Cursor cursor(path);
    parseValue(cursor);
}

void Parser::delegate(Parser& parser, const Path& path) {
    parser.depth = 0;
    parser.lexer = lexer;
    Path::Cursor cursor(path);
    parser.parseValue(cursor);
    lexer = parser.lexer;
    delegated = true;
}

Parser::Error::Error(Token token, const Lexer& lexer) :
    token(token), linePos(lexer.getTokenLineNumber()), charPos(lexer.getTokenCharPos()) {
    std::ostringstream s;
    s << "unexpected token " << token << " at line " << linePos << " (char " << charPos << ")";
    message = s.str();
}

void Parser::expectToken(Token expectedToken) {
    Token token = lexer.getNextToken();
    if (token != expectedToken) {
        throw Error(token, lexer);
    }
}

void Parser::parseValue(Path::Cursor& cursor) {
    parseValue(cursor, lexer.getNextToken());
}

void Parser::parseValue(Path::Cursor& cursor, Token token) {
    
    switch (token) {

        case Token::OBJECT_START:
            if (cursor.isInTarget()) onObjectStart();
            depth++;
            parseObject(cursor);
            depth--;
            if (cursor.isInTarget()) onObjectEnd();
            break;
        
        case Token::ARRAY_START:
            if (cursor.isInTarget()) onArrayStart();
            depth++;
            parseArray(cursor);
            depth--;
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

void Parser::parseObject(Path::Cursor& cursor) {

    Token token = lexer.getNextToken();
    
    switch (token) {

        case Token::OBJECT_END:
            break;
        
        case Token::STRING:
            expectToken(Token::COLON);
            if (cursor.isInTarget()) {
                cursor.next(lexer.getStringValue());
                onKey(lexer.getStringValue());
            } else {
                cursor.next(lexer.getStringValue());
            }
            if (!delegated) {
                parseValue(cursor);
            }
            delegated = false;
            cursor.prev();
            return parseNonEmptyObject(cursor);

        default:
            throw Error(token, lexer);
    }
}

void Parser::parseNonEmptyObject(Path::Cursor& cursor) {

    Token token = lexer.getNextToken();
    
    switch (token) {

        case Token::OBJECT_END:
            break;
        
        case Token::COMMA:
            expectToken(Token::STRING);
            expectToken(Token::COLON);
            if (cursor.isInTarget()) {
                cursor.next(lexer.getStringValue());
                onKey(lexer.getStringValue());
            } else {
                cursor.next(lexer.getStringValue());
            }
            if (!delegated) {
                parseValue(cursor);
            }
            delegated = false;
            cursor.prev();
            return parseNonEmptyObject(cursor);

        default:
            throw Error(token, lexer);
    }
}

void Parser::parseArray(Path::Cursor& cursor) {

    Token token = lexer.getNextToken();

    switch (token) {

        case Token::ARRAY_END:
            break;

        default:
            if (cursor.isInTarget()) {
                cursor.next(0);
                onIndex(0);
            } else {
                cursor.next(0);
            }
            if (!delegated) {
                parseValue(cursor, token);
            }
            delegated = false;
            cursor.prev();
            return parseNonEmptyArray(cursor);
    }
}

void Parser::parseNonEmptyArray(Path::Cursor& cursor, size_t index) {

    Token token = lexer.getNextToken();
    
    switch (token) {

        case Token::ARRAY_END:
            break;
        
        case Token::COMMA:
            if (cursor.isInTarget()) {
                cursor.next(index);
                onIndex(index);
            } else {
                cursor.next(index);
            }
            if (!delegated) {
                parseValue(cursor);
            }
            delegated = false;
            cursor.prev();
            return parseNonEmptyArray(cursor, index + 1);

        default:
            throw Error(token, lexer);
    }
}

}