#include <json/printer.h>
#include <cctype>

namespace JSON {

Printer::Printer(std::ostream& output, unsigned int indent, bool escapeUnicode, bool color) :
    output(output), indent(indent), escapeUnicode(escapeUnicode), color(color) {}

void Printer::setColor(const char* code) {
    if (color) {
        output << "\x1B[" << code << 'm';
    }
}

void Printer::printComma() {
    if (comma) {
        output << ',';
    }
}

void Printer::printTabs() {
    if (indent > 0) {
        if (colon) {
            colon = false;
            output << ' ';
        }
        else {
            if (root) {
                root = false;
            } else {
                output << '\n';
            }
            for (int i = 0; i < tabs * indent; i++) {
                output << ' ';
            }
        }
    }
}

void Printer::printChar(const unsigned char* str, int& i) {

    static const char* hexChars = "0123456789ABCDEF";

    if (escapeUnicode) {

        if (str[i] < 0b10000000) {
            if (isprint(str[i])) {
                output << str[i];
            }
            else {
                output << "\\u00" << hexChars[str[i] >> 4] << hexChars[str[i] & 0x000F];
            }
        }
        
        else if (str[i] < 0b11100000) {
            int code = str[i] & 0b00011111;
            if (str[++i] == '\0') { i--; return; }
            code = (code << 6) | (str[i] & 0b00111111);
            output << "\\u0" << hexChars[code >> 8] << hexChars[(code & 0x00F0) >> 4] << hexChars[code & 0x000F];
        }

        else {
            int code = str[i] & 0b00001111;
            if (str[++i] == '\0') { i--; return; }
            code = (code << 6) | (str[i] & 0b00111111);
            if (str[++i] == '\0') { i--; return; }
            code = (code << 6) | (str[i] & 0b00111111);
            output << "\\u" << hexChars[code >> 12] << hexChars[(code & 0x0F00) >> 8] << hexChars[(code & 0x00F0) >> 4] << hexChars[code & 0x000F];
        }
    }

    else {
        output << str[i];
    }
}

void Printer::printString(const char* str) {
    setColor("92");
    output << '\"';
    for (int i = 0; str[i] != '\0'; i++) {
        switch (str[i]) {
            case '\"': output << "\\\""; break;
            case '\\': output << "\\\\"; break;
            case '\b': output << "\\b"; break;
            case '\f': output << "\\f"; break;
            case '\n': output << "\\n"; break;
            case '\r': output << "\\r"; break;
            case '\t': output << "\\t"; break;
            default:
                printChar((const unsigned char*)str, i);
                break;
        }
    }
    output << '\"';
    setColor();
}

void Printer::startObject() {
    printComma();
    printTabs();
    tabs++;
    comma = false;
    output << '{';
}

void Printer::endObject() {
    tabs--;
    printTabs();
    comma = true;
    output << '}';
}

void Printer::startArray() {
    printComma();
    printTabs();
    tabs++;
    comma = false;
    output << '[';
}

void Printer::endArray() {
    tabs--;
    printTabs();
    comma = true;
    output << ']';
}

void Printer::key(const std::string& key) {
    printComma();
    printTabs();
    colon = true;
    comma = false;
    printString(key.c_str());
    output << ':';
}

void Printer::value(const char* value) {
    printComma();
    printTabs();
    comma = true;
    printString(value);
}

void Printer::value(const std::string& value) {
    Printer::value(value.c_str());
}

void Printer::value(double value) {
    printComma();
    printTabs();
    comma = true;
    setColor("93");
    output << value;
    setColor();
}

void Printer::value(bool value) {
    printComma();
    printTabs();
    comma = true;
    setColor("95");
    output << (value ? "true" : "false");
    setColor();
}

void Printer::value() {
    printComma();
    printTabs();
    comma = true;
    setColor("95");
    output << "null";
    setColor();
}

}