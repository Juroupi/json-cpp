#ifndef _JSON_PATH_H_
#define _JSON_PATH_H_

#include <string>
#include <vector>
#include <memory>

namespace JSON {

/**
 * Represents a path to a value in a JSON data tree (inspired by JSONPath).
 * A path is a list of successive elements, representing each step to reach the target value.
 * An element can be either an index for arrays or a key for objects.
 * An element can also be deep scan, which means that it can match deeper values in the tree.
 */
class Path {

public:

    struct Element;

    class Cursor;

    /**
     * An empty structure to be used as a parameter to the Path::operator<< method.
     */
    struct DeepScan {};

private:

    enum class Token;
    struct Lexer;
    class Parser;

    bool deepScanNext = false;

    std::vector<std::unique_ptr<Element>> elements;

public:

    /**
     * Creates an empty path.
     */
    Path() = default;

    /**
     * Creates a copy of the given path.
     */
    Path(const Path& path);

    /**
     * Moves the given path to a new path.
     */
    Path(Path&& path);
    
    /**
     * Creates a path from a string representation.
     * Supports the following syntax :
     *  - `$` : root element (optional)
     *  - `['<string>']` : object key
     *  - `[<number>]` : array index
     *  - `[<number>:<number>:<number>]` : start, end and step (optional = 1) for a range of indexes
     *  - `[<string or number>, ...]` : one of the given key or index
     *  - `[*]` : any object key or array index
     *  - `.` : deep scan for the following element
     * std::runtime_error is thrown if the syntax is invalid.
     * To create more complex paths (e.g. with functions), use the Path::operator<< method.
     */
    Path(const char* path);
    Path(const std::string& path);

    /**
     * Get the number of elements in the path.
     */
    size_t getSize() const;

    /**
     * Get the element at the given index.
     */
    const Element& operator[](size_t index) const;

    /**
     * Add an element at the end of the path.
     */
    Path& operator<<(std::unique_ptr<Element>&& element);

    /**
     * Specify that the next element to be added to the path should be deep scan.
     */
    Path& operator<<(DeepScan);
};

}

std::ostream& operator<<(std::ostream& output, const JSON::Path& path);

#include <json/path/element.h>

#endif