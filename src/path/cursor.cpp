#include <json/path/cursor.h>

namespace JSON {

Path::Cursor::Cursor(const Path& path) :
    path(path), matches(path.getSize(), 0) {}

void Path::Cursor::next(const std::string& name) {
    if (diff > 0) {
        diff++;
    } else if (cursor >= path.getSize()) {
        cursor++;
    } else if (path[cursor].accept(name)) {
        matches[cursor]++;
        totalMatches++;
        cursor++;
    } else if (path[cursor].deepScan) {
        matches[cursor]++;
        totalMatches++;
    } else {
        diff++;
    }
    depth++;
}

void Path::Cursor::next(size_t index) {
    if (diff > 0) {
        diff++;
    } else if (cursor >= path.getSize()) {
        cursor++;
    } else if (path[cursor].accept(index)) {
        matches[cursor]++;
        totalMatches++;
        cursor++;
    } else if (path[cursor].deepScan) {
        matches[cursor]++;
        totalMatches++;
    } else {
        diff++;
    }
    depth++;
}

void Path::Cursor::prev() {
    depth--;
    if (diff > 0) {
        diff--;
    } else if (cursor > path.getSize()) {
        cursor--;
    } else if (cursor == path.getSize() || matches[cursor] == 0) {
        cursor--;
        totalMatches--;
        matches[cursor]--;
    } else {
        totalMatches--;
        matches[cursor]--;
    }
}

void Path::Cursor::reset() {
    cursor = 0;
    depth = 0;
    diff = 0;
    totalMatches = 0;
    matches.clear();
    matches.resize(path.getSize(), 0);
}

bool Path::Cursor::isInPath() const {
    return diff == 0;
}

bool Path::Cursor::isInTarget() const {
    return cursor >= path.getSize();
}

}