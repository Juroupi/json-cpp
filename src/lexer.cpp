#include <json/lexer.h>
#include <sstream>
#include <cmath>
#include <cctype>

namespace JSON {

Lexer::Lexer() :
    input(nullptr),
    token(Token::END_OF_STREAM) {}

Lexer::Lexer(std::istream& input) {
    setInput(input);
}

void Lexer::setInput(std::istream& input) {
    this->input = &input;
    charPos = 0;
    lineNumber = 1;
    tokenCharPos = charPos;
    tokenLineNumber = lineNumber;
    carriageReturn = false;
    nextToken();
}

double Lexer::getNumberValue() const {
    return numberValue;
}

const std::string& Lexer::getStringValue() const {
    return stringValue;
}

std::string& Lexer::getStringValue() {
    return stringValue;
}

bool Lexer::getBooleanValue() const {
    return booleanValue;
}

int Lexer::getCharPos() const {
    return charPos;
}

int Lexer::getLineNumber() const {
    return lineNumber;
}

int Lexer::getTokenCharPos() const {
    return tokenCharPos;
}

int Lexer::getTokenLineNumber() const {
    return tokenLineNumber;
}

void Lexer::nextLine() {
    charPos = 0;
    lineNumber++;
}

char Lexer::getNextChar() {
    char c;
    if (input != nullptr && input->get(c)) {
        charPos++;
        return c;
    }
    return '\0';
}

Lexer::Error::Error(Code code, const Lexer& lexer, char c) : code(code), lineNumber(lexer.getLineNumber()), charPos(lexer.getCharPos()), c(c) {

    std::ostringstream s;

    switch (code) {
        case INVALID_CHARACTER: s << "invalid character "; if (isprint(c)) s << "'" << c << "'"; else s << (int)c; break;
        case INVALID_NUMBER: s << "invalid number"; break;
        case UNTERMINATED_STRING: s << "unterminated string"; break;
        case INVALID_ESCAPE_SEQUENCE: s << "invalid escape sequence '\\" << c << "'"; break;
        case INVALID_UNICODE_ESCAPE_SEQUENCE: s << "invalid unicode escape sequence"; break;
        default: s << "unknown error"; break;
    }

    s << " at line " << lineNumber << " (char " << charPos << ")";

    message = s.str();
}

Token Lexer::getNextToken() {

    char c = getNextChar();

    if (c == ' ' || c == '\t') {
        return getNextToken();
    }

    if (c == '\r') {
        carriageReturn = true;
        return nextLine(), getNextToken();
    }

    if (c == '\n') {
        if (carriageReturn) {
            carriageReturn = false;
            charPos--;
            return getNextToken();
        }
        return nextLine(), getNextToken();
    }
    
    tokenCharPos = charPos;
    tokenLineNumber = lineNumber;

    if (c == '\0') {
        return Token::END_OF_STREAM;
    }

    if (c == '{') {
        return Token::OBJECT_START;
    }

    if (c == '}') {
        return Token::OBJECT_END;
    }
    
    if (c == '[') {
        return Token::ARRAY_START;
    }
    
    if (c == ']') {
        return Token::ARRAY_END;
    }
    
    if (c == ':') {
        return Token::COLON;
    }
    
    if (c == ',') {
        return Token::COMMA;
    }

    if (c == '\"') {
        return getNextString(), Token::STRING;
    }

    if (c == '-' || (c >= '0' && c <= '9')) {
        return getNextNumber(c), Token::NUMBER;
    }

    if (c == 't') {
        if (getNextChar() == 'r' && getNextChar() == 'u' && getNextChar() == 'e') {
            return booleanValue = true, Token::BOOLEAN;
        }
    }

    else if (c == 'f') {
        if (getNextChar() == 'a' && getNextChar() == 'l' && getNextChar() == 's' && getNextChar() == 'e') {
            return booleanValue = false, Token::BOOLEAN;
        }
    }

    else if (c == 'n') {
        if (getNextChar() == 'u' && getNextChar() == 'l' && getNextChar() == 'l') {
            return Token::NULL_;
        }
    }

    throw Error(Error::INVALID_CHARACTER, *this, c);
}

void Lexer::nextToken() {
    token = getNextToken();
}

Token Lexer::getToken() const {
    return token;
}

void Lexer::getNextNumber(char c) {

    double sign = 1, intPart = 0, fracPart = 0, exponent = 0, exponentSign = 1;

    if (c == '-') {
        sign = -1;
        c = getNextChar();
    }

    if (c == '0') {
        c = getNextChar();
    }
    else if (c >= '1' && c <= '9') {
        do {
            intPart = (intPart * 10) + (c - '0');
            c = getNextChar();
        } while (c >= '0' && c <= '9');
    }
    else {
        throw Error(Error::INVALID_NUMBER, *this);
    }

    if (c == '.') {
        c = getNextChar();
        if (c >= '0' && c <= '9') {
            double prec = 10;
            do {
                fracPart += (c - '0') / prec;
                prec *= 10;
                c = getNextChar();
            } while (c >= '0' && c <= '9');
        }
        else {
            throw Error(Error::INVALID_NUMBER, *this);
        }
    }

    if (c == 'e' || c == 'E') {

        c = getNextChar();

        if (c == '-') {
            exponentSign = -1;
            c = getNextChar();
        }
        else if (c == '+') {
            c = getNextChar();
        }

        if (c >= '0' && c <= '9') {
            do {
                exponent = (exponent * 10) + (c - '0');
                c = getNextChar();
            } while (c >= '0' && c <= '9');
        }
        else {
            throw Error(Error::INVALID_NUMBER, *this);
        }
    }

    numberValue = sign * (intPart + fracPart) * pow(10, exponentSign * exponent);

    charPos--;
    
    input->unget();
}

void Lexer::getNextString() {

    stringValue.clear();

    bool escape = false;

    while (true) {

        char c = getNextChar();

        if (c == '\0') {
            throw Error(Error::UNTERMINATED_STRING, *this);
        }

        else if (escape) {

            if (c == '\"') {
                stringValue += '\"';
            }
            else if (c == '\\') {
                stringValue += '\\';
            }
            else if (c == '/') {
                stringValue += '/';
            }
            else if (c == 'b') {
                stringValue += '\b';
            }
            else if (c == 'f') {
                stringValue += '\f';
            }
            else if (c == 'n') {
                stringValue += '\n';
            }
            else if (c == 'r') {
                stringValue += '\r';
            }
            else if (c == 't') {
                stringValue += '\t';
            }

            else if (c == 'u') {

                unsigned int code = 0;

                for (int i = 0; i < 4; i++) {

                    c = getNextChar();

                    if (c >= '0' && c <= '9') {
                        code = (code << 4) | (c - '0');
                    }
                    else if (c >= 'A' && c <= 'F') {
                        code = (code << 4) | (c - 'A' + 10);
                    }
                    else if (c >= 'a' && c <= 'f') {
                        code = (code << 4) | (c - 'a' + 10);
                    }
                    else {
                        throw Error(Error::INVALID_UNICODE_ESCAPE_SEQUENCE, *this);
                    }
                }

                if (code <= 0x7F) {
                    stringValue += (char)code;
                }
                else if (code <= 0x7FF) {
                    stringValue += (char)(0xC0 | (code >> 6));
                    stringValue += (char)(0x80 | (code & 0x3F));
                }
                else {
                    stringValue += (char)(0xE0 | (code >> 12));
                    stringValue += (char)(0x80 | ((code >> 6) & 0x3F));
                    stringValue += (char)(0x80 | (code & 0x3F));
                }
            }

            else {
                throw Error(Error::INVALID_ESCAPE_SEQUENCE, *this);
            }

            escape = false;
        }

        else if (c == '\\') {
            escape = true;
        }

        else if (c == '\"') {
            return;
        }

        else if (c == '\b' || c == '\f' || c == '\n' || c == '\r' || c == '\t') {
            throw Error(Error::INVALID_CHARACTER, *this, c);
        }

        else {
            stringValue += c;
        }
    }
}

};

std::ostream& operator<<(std::ostream& stream, JSON::Token token) {
    
    switch (token) {
        case JSON::Token::OBJECT_START: return stream << "OBJECT_START";
        case JSON::Token::OBJECT_END: return stream << "OBJECT_END";
        case JSON::Token::ARRAY_START: return stream << "ARRAY_START";
        case JSON::Token::ARRAY_END: return stream << "ARRAY_END";
        case JSON::Token::COLON: return stream << "COLON";
        case JSON::Token::COMMA: return stream << "COMMA";
        case JSON::Token::NUMBER: return stream << "NUMBER";
        case JSON::Token::STRING: return stream << "STRING";
        case JSON::Token::BOOLEAN: return stream << "BOOLEAN";
        case JSON::Token::NULL_: return stream << "NULL";
        case JSON::Token::END_OF_STREAM: return stream << "END_OF_STREAM";
        default: return stream << "ERROR";
    }
}