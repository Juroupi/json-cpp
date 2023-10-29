#ifndef _JSON_VALUE_H_
#define _JSON_VALUE_H_

#include <string>
#include <vector>
#include <map>
#include <istream>
#include <ostream>
#include <JSON/error.h>
#include <JSON/printer.h>
#include <JSON/path.h>
#include <json/path/cursor.h>

namespace JSON {

class Value;

/**
 * Alias for the different JSON value types.
 */
using Number = double;
using Boolean = bool;
using String = std::string;
using Null = std::nullptr_t;
using Object = std::map<std::string, Value>;
using Array = std::vector<Value>;

/**
 * Enum for different JSON value type IDs.
 * Undefined is not a valid type but is used internally.
 */
enum class Type {
    UNDEFINED,
    NUMBER,
    BOOLEAN,
    NULL_,
    STRING,
    OBJECT,
    ARRAY
};

/**
 * The null value.
 */
static constexpr const Null null = Null();

/**
 * A JSON value.
 */
class Value {

    union {
        Number numberValue;
        Boolean booleanValue;
        Null nullValue;
        String stringValue;
        Object objectValue;
        Array arrayValue;
    };

    Type type;

    void clearValue();
    void assignValue(const Value& value);
    void assignValue(Value&& value);

    const Value* findFirst(Path::Cursor& cursor) const;

    void findAll(std::vector<Value*>& all, Path::Cursor& cursor);
    void findAll(std::vector<const Value*>& all, Path::Cursor& cursor) const;

public:

    /**
     * Exception thrown when a type assertion fails.
     * For example, when trying to get a number from a string value.
     */
    struct TypeAssertionError : JSON::Error {
        Type type;
        TypeAssertionError(Type type);
    };

    /**
     * Exception thrown when a key is not found in an object or an index is out of bounds in an array.
     */
    struct KeyError : JSON::Error {
        std::string key;
        KeyError(const std::string& type);
    };

    /**
     * Exception thrown when trying to use an undefined value.
     */
    struct UndefinedValueError : JSON::Error {
        UndefinedValueError();
    };

    /**
     * Creates an undefined value.
     */
    Value();

    /**
     * Creates a value from another value.
     */
    Value(const Value& value);
    Value(Value&& value);

    /**
     * Creates a value from a primitive value.
     */
    Value(Number value);
    Value(Boolean value);
    Value(Null value);
    Value(const String& value);
    Value(String&& value);
    Value(const Object& value);
    Value(Object&& value);
    Value(const Array& value);
    Value(Array&& value);

    /**
     * Destroys the value.
     */
    ~Value();

    /**
     * Clears the value.
     * After this method is called, the value is undefined.
     */
    void clear();

    /**
     * Assigns a value to this value.
     */
    void assign(const Value& value);
    void assign(Value&& value);

    /**
     * Returns the type of the value.
     */
    Type getType() const;

    /**
     * Returns true if the value is of the given type.
     */
    bool hasType(Type type) const;

    /**
     * Returns true if the value is undefined.
     */
    bool isUndefined() const;

    /**
     * Throws a TypeAssertionError exception if the value is not of the given type.
     */
    void assertType(Type type) const;

    /**
     * Returns the value as the given type.
     * Throws a TypeAssertionError exception if the value is not of the given type.
     */
    Number& getNumberValue();
    Number getNumberValue() const;
    Boolean& getBooleanValue();
    Boolean getBooleanValue() const;
    Null& getNullValue();
    Null getNullValue() const;
    String& getStringValue();
    const String& getStringValue() const;
    Object& getObjectValue();
    const Object& getObjectValue() const;
    Array& getArrayValue();
    const Array& getArrayValue() const;

    /**
     * Sets the value to the given value.
     */
    void setNumberValue(Number value = Number());
    void setBooleanValue(Boolean value = Boolean());
    void setNullValue(Null value = Null());
    void setStringValue(const String& value);
    void setStringValue(String&& value = String());
    void setObjectValue(const Object& value);
    void setObjectValue(Object&& value = Object());
    void setArrayValue(const Array& value);
    void setArrayValue(Array&& value = Array());

    /**
     * Assigns a value to this value.
     * Returns a reference to the this value.
     */
    Value& operator=(const Value& value);
    Value& operator=(Value&& value);

    /**
     * Sets the value to the given value.
     * Returns a reference to the this value.
     */
    Value& operator=(Number value);
    Value& operator=(Boolean value);
    Value& operator=(Null value);
    Value& operator=(const String& value);
    Value& operator=(String&& value);
    Value& operator=(const Object& value);
    Value& operator=(Object&& value);
    Value& operator=(const Array& value);
    Value& operator=(Array&& value);

    /**
     * Returns the child value at the given key for an object.
     * Throws a TypeAssertionError exception if the value is not an object.
     * Throws a KeyError exception if the key is not found.
     */
    Value& operator[](const String& key);
    const Value& operator[](const String& key) const;

    /**
     * Returns the value at the given index for an array.
     * Throws a TypeAssertionError exception if the value is not an array.
     * Throws a KeyError exception if the key is not found.
     */
    Value& operator[](size_t index);
    const Value& operator[](size_t index) const;

    /**
     * Compares two values.
     * If the values are not of the same type, they are considered different.
     * If the values have the same type, they are compared using the comparison operators for their primitive values.
     */
    bool operator==(const Value& value) const;
    bool operator!=(const Value& value) const;

    /**
     * Prints the value using the given printer.
     */
    void print(Printer& printer) const;

    /**
     * Creates a printer with the given parameters and prints the value.
     */
    void print(std::ostream& output, int indent = 0, bool escapeUnicode = true, bool color = false) const;

    /**
     * Parses a value from the given input stream.
     * Clears the current value before parsing.
     * Only the values matching the given path are retained.
     * If unique is true, only the last value matching the path is retained.
     * If unique is false, all values matching the path are retained as an array.
     * If an object contains multiple values with the same key in the target value, only the last one is retained.
     */
    void parse(std::istream& input, const Path& path = {}, bool unique = true);

    /**
     * Finds the first sub-value matching the given path.
     * Returns nullptr if no value is found.
     */
    Value* findFirst(const Path& path);
    const Value* findFirst(const Path& path) const;

    /**
     * Finds all sub-values matching the given path.
     */
    std::vector<Value*> findAll(const Path& path);
    std::vector<const Value*> findAll(const Path& path) const;
};

/**
 * Parses a value from the given string using Value::parse. 
 */
Value parse(const std::string& json, const Path& path = {}, bool unique = true);

/**
 * Parses a value from the file at the given path using Value::parse.
 */
Value load(const std::string& file, const Path& path = {}, bool unique = true);

}

/**
 * Prints the name of the given type.
 */
std::ostream& operator<<(std::ostream& output, JSON::Type type);

/**
 * Parses a value using Value::parse()
 */
std::istream& operator>>(std::istream& input, JSON::Value& value);

/**
 * Prints the value using Value::print()
 */
std::ostream& operator<<(std::ostream& output, const JSON::Value& value);

#endif