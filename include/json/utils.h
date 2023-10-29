#ifndef _JSON_UTILS_H_
#define _JSON_UTILS_H_

#include <json/path.h>
#include <istream>
#include <ostream>

namespace JSON {

/**
 * Copies JSON data from an input stream to an output stream.
 * The syntax is checked and the data is formatted.
 * The ident parameter is the number of spaces to use for each indentation level.
 * If ident is 0, the ouput contains no indentation, extra spaces or newlines.
 * If escapeUnicode is true, unicode characters are escaped as \uXXXX.
 * If path is not empty, only the values in the path are copied.
 */
void copy(std::ostream& output, std::istream& input, int ident = 0, bool escapeUnicode = true, const Path& path = {});

/**
 * Reads JSON data from an input stream and checks the syntax.
 */
bool check(std::istream& input);

}

#endif