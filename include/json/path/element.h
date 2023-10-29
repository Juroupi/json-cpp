#ifndef _JSON_PATH_ELEMENT_H_
#define _JSON_PATH_ELEMENT_H_

#include <json/path.h>
#include <string>
#include <utility>
#include <ostream>
#include <functional>

namespace JSON {

/**
 * An element of a path.
 */
struct Path::Element {

    bool deepScan = false;

    virtual ~Element() = default;

    virtual bool accept(const std::string& name) const;
    virtual bool accept(size_t index) const;
    virtual void print(std::ostream& output) const = 0;

    struct Name;
    struct Index;
    struct Range;
    struct Options;
    struct Any;
    struct Function;
};

/**
 * An element that matches a given object key.
 */
struct Path::Element::Name : Path::Element, std::string {
    using std::string::string;
    bool accept(const std::string& name) const override;
    void print(std::ostream& output) const override;
};

/**
 * An element that matches a given array index.
 */
struct Path::Element::Index : Path::Element {
    size_t index;
    Index(size_t index);
    bool accept(size_t index) const override;
    void print(std::ostream& output) const override;
};

/**
 * An element that matches a range of array indexes.
 */
struct Path::Element::Range : Path::Element {
    size_t first, last, step;
    Range(size_t first, size_t last, size_t step = 1);
    bool accept(size_t index) const override;
    void print(std::ostream& output) const override;
};

/**
 * Represents a path element that matches one of a set of elements.
 */
struct Path::Element::Options : Path::Element, std::pair<std::unique_ptr<Element>, std::unique_ptr<Element>> {
    using std::pair<std::unique_ptr<Element>, std::unique_ptr<Element>>::pair;
    bool accept(const std::string& name) const override;
    bool accept(size_t index) const override;
    void print(std::ostream& output) const override;
};

/**
 * An element that matches any object key or array index.
 */
struct Path::Element::Any : Path::Element {
    bool accept(const std::string& name) const override;
    bool accept(size_t index) const override;
    void print(std::ostream& output) const override;
};

/**
 * An element that matches custom criteria.
 * The first function is used to match object keys.
 * The second function is used to match array indexes.
 */
struct Path::Element::Function : Path::Element, std::pair<std::function<bool(const std::string&)>, std::function<bool(size_t)>> {
    using std::pair<std::function<bool(const std::string&)>, std::function<bool(size_t)>>::pair;
    bool accept(const std::string& name) const override;
    bool accept(size_t index) const override;
    void print(std::ostream& output) const override;
};

}

#endif