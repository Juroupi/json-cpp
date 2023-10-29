#ifndef _JSON_PRINTER_H_
#define _JSON_PRINTER_H_

#include <string>
#include <ostream>

namespace JSON {

/**
 * A class to easily print JSON data in a stream.
 * No syntax check is done.
 */
class Printer {

    std::ostream& output;
    unsigned int tabs = 0, indent;
    bool escapeUnicode, color;
    bool root = true, comma = false, colon = false;

    void setColor(const char* code = "0");
    void printComma();
    void printTabs();
    void printString(const char* str);
    void printChar(const unsigned char* str, int& i);

public:

    /**
     * Creates a printer for a given stream.
     * The indent parameter is the number of spaces to use for each indentation level.
     * If indent is 0, the ouput contains no indentation, extra spaces or newlines.
     * If escapeUnicode is true, unicode characters are escaped as \uXXXX.
     * If color is true, the output is colored with ANSI escape codes.
     */
    Printer(std::ostream& output, unsigned int indent = 0, bool escapeUnicode = true, bool color = false);

    /**
     * Prints object delimiters.
     */
    void startObject();
    void endObject();

    /**
     * Prints array delimiters.
     */
    void startArray();
    void endArray();

    /**
     * Prints an object key.
     * Do not use this to print a string value or vice-versa.
     */
    void key(const std::string& key);

    /**
     * Prints a value.
     */
    void value(const char* value);
    void value(const std::string& value);
    void value(double value);
    void value(bool value);
    void value();
};

}

#endif