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

Parser::Error::Error(const Lexer& lexer) :
    token(lexer.getToken()), linePos(lexer.getTokenLineNumber()), charPos(lexer.getTokenCharPos()) {
    std::ostringstream s;
    s << "unexpected token " << token << " at line " << linePos << " (char " << charPos << ")";
    message = s.str();
}

void Parser::expectToken(Token expectedToken) {
    if (lexer.getToken() != expectedToken) {
        throw Error(lexer);
    }
    lexer.nextToken();
}

void Parser::parseValue(Path::Cursor& cursor) {
    
    switch (lexer.getToken()) {

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
            lexer.nextToken();
            break;

        case Token::BOOLEAN:
            if (cursor.isInTarget()) onBoolean(lexer.getBooleanValue());
            lexer.nextToken();
            break;
        
        case Token::STRING:
            if (cursor.isInTarget()) onString(lexer.getStringValue());
            lexer.nextToken();
            break;

        case Token::NULL_:
            if (cursor.isInTarget()) onNull();
            lexer.nextToken();
            break;
        
        default:
            throw Error(lexer);
    }
}

void Parser::parseObject(Path::Cursor& cursor) {

    lexer.nextToken();
    
    switch (lexer.getToken()) {

        case Token::OBJECT_END:
            lexer.nextToken();
            break;

        case Token::STRING:
            {
                std::string key = std::move(lexer.getStringValue());
                lexer.nextToken();
                expectToken(Token::COLON);
                if (cursor.isInTarget()) {
                    cursor.next(key);
                    onKey(key);
                } else {
                    cursor.next(key);
                }
            }
            if (!delegated) {
                parseValue(cursor);
            }
            delegated = false;
            cursor.prev();
            return parseNonEmptyObject(cursor);

        default:
            throw Error(lexer);
    }
}

void Parser::parseNonEmptyObject(Path::Cursor& cursor) {
    
    switch (lexer.getToken()) {

        case Token::OBJECT_END:
            lexer.nextToken();
            break;

        case Token::COMMA:
            lexer.nextToken();
            {
                std::string key = std::move(lexer.getStringValue());
                expectToken(Token::STRING);
                expectToken(Token::COLON);
                if (cursor.isInTarget()) {
                    cursor.next(key);
                    onKey(key);
                } else {
                    cursor.next(key);
                }
            }
            if (!delegated) {
                parseValue(cursor);
            }
            delegated = false;
            cursor.prev();
            return parseNonEmptyObject(cursor);

        default:
            throw Error(lexer);
    }
}

void Parser::parseArray(Path::Cursor& cursor) {

    lexer.nextToken();

    switch (lexer.getToken()) {

        case Token::ARRAY_END:
            lexer.nextToken();
            break;

        default:
            if (cursor.isInTarget()) {
                cursor.next(0);
                onIndex(0);
            } else {
                cursor.next(0);
            }
            if (!delegated) {
                parseValue(cursor);
            }
            delegated = false;
            cursor.prev();
            return parseNonEmptyArray(cursor);
    }
}

void Parser::parseNonEmptyArray(Path::Cursor& cursor, size_t index) {

    switch (lexer.getToken()) {

        case Token::ARRAY_END:
            lexer.nextToken();
            break;
        
        case Token::COMMA:
            lexer.nextToken();
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
            throw Error(lexer);
    }
}

}