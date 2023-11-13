#ifndef _JSON_PATH_CURSOR_H_
#define _JSON_PATH_CURSOR_H_

#include <json/path.h>
#include <string>
#include <vector>

namespace JSON {

/**
 * A cursor to navigate through a JSON data tree.
 * The cursor is used to check if a value is in the path or in the target value during a search.
 * If the path is modified, the cursor is invalid until reset() is called.
 */
class Path::Cursor {

    const Path& path;

    std::vector<size_t> matches;

    size_t cursor = 0;
    size_t depth = 0;
    size_t diff = 0;
    size_t totalMatches = 0;

public:

    /**
     * Creates a cursor for a given path.
     */
    Cursor(const Path& path);

    /**
     * Moves the cursor to the child of the current value with the given name or index.
     */
    void next(const std::string& name);
    void next(size_t index);
    
    /**
     * Moves the cursor to parent of the current value.
     */
    void prev();

    /**
     * Resets the cursor to the root of the path.
     */
    void reset();

    /**
     * Test if the current value is in the path.
     */
    bool isInPath() const;

    /**
     * Test if the current value is in the target value.
     * If it is true, isInPath() is also true.
     */
    bool isInTarget() const;

    /**
     * Test if the current value is the root of the target value.
     */
    bool isInTargetRoot() const;
};

}

#endif