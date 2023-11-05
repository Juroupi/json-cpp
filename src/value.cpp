#include <json/value.h>
#include <json/parser.h>
#include <sstream>
#include <fstream>

namespace JSON {

Value::TypeAssertionError::TypeAssertionError(Type type) : type(type) {
    std::ostringstream s;
    s << "value type is not " << type;
    message = s.str();
}

Value::KeyError::KeyError(const std::string& key) : key(key) {
    std::ostringstream s;
    s << "value does not contain key '" << key << "'";
    message = s.str();
}

Value::UndefinedValueError::UndefinedValueError() {
    message = "value is undefined";
}

void Value::clearValue() {
    switch (type) {
        case Type::STRING: stringValue.~String(); break;
        case Type::OBJECT: objectValue.~Object(); break;
        case Type::ARRAY: arrayValue.~Array(); break;
    }
}

void Value::assignValue(const Value& value) {
    switch (type = value.type) {
        case Type::NUMBER: numberValue = value.numberValue; break;
        case Type::BOOLEAN: booleanValue = value.booleanValue; break;
        case Type::NULL_: nullValue = value.nullValue; break;
        case Type::STRING: new (&stringValue) String(value.stringValue); break;
        case Type::OBJECT: new (&objectValue) Object(value.objectValue); break;
        case Type::ARRAY: new (&arrayValue) Array(value.arrayValue); break;
    }
}

void Value::assignValue(Value&& value) {
    switch (type = value.type) {
        case Type::NUMBER: numberValue = value.numberValue; break;
        case Type::BOOLEAN: booleanValue = value.booleanValue; break;
        case Type::NULL_: nullValue = value.nullValue; break;
        case Type::STRING: new (&stringValue) String(std::move(value.stringValue)); break;
        case Type::OBJECT: new (&objectValue) Object(std::move(value.objectValue)); break;
        case Type::ARRAY: new (&arrayValue) Array(std::move(value.arrayValue)); break;
    }
}

Value::Value() : type(Type::UNDEFINED) {}
Value::Value(const Value& value) { assignValue(value); }
Value::Value(Value&& value) { assignValue(std::move(value)); }

Value::Value(Number value) : type(Type::NUMBER), numberValue(value) {}
Value::Value(Boolean value) : type(Type::BOOLEAN), booleanValue(value) {}
Value::Value(Null value) : type(Type::NULL_), nullValue(value) {}
Value::Value(const String& value) : type(Type::STRING), stringValue(value) {}
Value::Value(String&& value) : type(Type::STRING), stringValue(std::move(value)) {}
Value::Value(const Object& value) : type(Type::OBJECT), objectValue(value) {}
Value::Value(Object&& value) : type(Type::OBJECT), objectValue(std::move(value)) {}
Value::Value(const Array& value) : type(Type::ARRAY), arrayValue(value) {}
Value::Value(Array&& value) : type(Type::ARRAY), arrayValue(std::move(value)) {}

Value::~Value() { clearValue(); }

Type Value::getType() const { return type; }
bool Value::hasType(Type type) const { return this->type == type; }
bool Value::isUndefined() const { return this->type == Type::UNDEFINED; }
void Value::assertType(Type type) const { if (this->type != type) throw TypeAssertionError(type); }

void Value::clear() { clearValue(); type = Type::UNDEFINED; }
void Value::assign(const Value& value) {
    if (type == value.type) {
        switch (type) {
            case Type::NUMBER: numberValue = value.numberValue; break;
            case Type::BOOLEAN: booleanValue = value.booleanValue; break;
            case Type::NULL_: nullValue = value.nullValue; break;
            case Type::STRING: stringValue = value.stringValue; break;
            case Type::OBJECT: objectValue = value.objectValue; break;
            case Type::ARRAY: arrayValue = value.arrayValue; break;
        }
    } else {
        clearValue();
        assignValue(value);
    }
}
void Value::assign(Value&& value) {
    if (type == value.type) {
        switch (type) {
            case Type::NUMBER: numberValue = value.numberValue; break;
            case Type::BOOLEAN: booleanValue = value.booleanValue; break;
            case Type::NULL_: nullValue = value.nullValue; break;
            case Type::STRING: stringValue = std::move(value.stringValue); break;
            case Type::OBJECT: objectValue = std::move(value.objectValue); break;
            case Type::ARRAY: arrayValue = std::move(value.arrayValue); break;
        }
    } else {
        clearValue();
        assignValue(value);
    }
}

Number& Value::getNumberValue() { assertType(Type::NUMBER); return numberValue; }
Boolean& Value::getBooleanValue() { assertType(Type::BOOLEAN); return booleanValue; }
Null& Value::getNullValue() { assertType(Type::NULL_); return nullValue; }
String& Value::getStringValue() { assertType(Type::STRING); return stringValue; }
Object& Value::getObjectValue() { assertType(Type::OBJECT); return objectValue; }
Array& Value::getArrayValue() { assertType(Type::ARRAY); return arrayValue; }

Number Value::getNumberValue() const { assertType(Type::NUMBER); return numberValue; }
Boolean Value::getBooleanValue() const { assertType(Type::BOOLEAN); return booleanValue; }
Null Value::getNullValue() const { assertType(Type::NULL_); return nullValue; }
const String& Value::getStringValue() const { assertType(Type::STRING); return stringValue; }
const Object& Value::getObjectValue() const { assertType(Type::OBJECT); return objectValue; }
const Array& Value::getArrayValue() const { assertType(Type::ARRAY); return arrayValue; }

void Value::setNumberValue(Number value) { clearValue(); type = Type::NUMBER; numberValue = value; }
void Value::setBooleanValue(Boolean value) { clearValue(); type = Type::BOOLEAN; booleanValue = value; }
void Value::setNullValue(Null value) { clearValue(); type = Type::NULL_; nullValue = value; }
void Value::setStringValue(String&& value) { if (type == Type::STRING) { stringValue = std::move(value); } else { clearValue(); type = Type::STRING; new (&stringValue) String(std::move(value)); } }
void Value::setObjectValue(Object&& value) { if (type == Type::OBJECT) { objectValue = std::move(value); } else { clearValue(); type = Type::OBJECT; new (&objectValue) Object(std::move(value)); } }
void Value::setArrayValue(Array&& value) { if (type == Type::ARRAY) { arrayValue = std::move(value); } else { clearValue(); type = Type::ARRAY; new (&arrayValue) Array(std::move(value)); } }

void Value::setStringValue(const String& value) { if (type == Type::STRING) { stringValue = value; } else { clearValue(); type = Type::STRING; new (&stringValue) String(value); } }
void Value::setObjectValue(const Object& value) { if (type == Type::OBJECT) { objectValue = value; } else { clearValue(); type = Type::OBJECT; new (&objectValue) Object(value); } }
void Value::setArrayValue(const Array& value) { if (type == Type::ARRAY) { arrayValue = value; } else { clearValue(); type = Type::ARRAY; new (&arrayValue) Array(value); } }

Value& Value::operator=(const Value& value) { assign(value); return *this; }
Value& Value::operator=(Value&& value) { assign(std::move(value)); return *this; }

Value& Value::operator=(Number value) { setNumberValue(value); return *this; }
Value& Value::operator=(Boolean value) { setBooleanValue(value); return *this; }
Value& Value::operator=(Null value) { setNullValue(value); return *this; }
Value& Value::operator=(String&& value) { setStringValue(std::move(value)); return *this; }
Value& Value::operator=(Object&& value) { setObjectValue(std::move(value)); return *this; }
Value& Value::operator=(Array&& value) { setArrayValue(std::move(value)); return *this; }

Value& Value::operator=(const String& value) { setStringValue(value); return *this; }
Value& Value::operator=(const Object& value) { setObjectValue(value); return *this; }
Value& Value::operator=(const Array& value) { setArrayValue(value); return *this; }

bool Value::operator!=(const Value& value) const { return !(*this == value); }
bool Value::operator==(const Value& value) const {
    if (type == value.type) {
        switch (type) {
            case Type::NUMBER: return numberValue == value.numberValue;
            case Type::BOOLEAN: return booleanValue == value.booleanValue;
            case Type::NULL_: return true;
            case Type::STRING: return stringValue == value.stringValue;
            case Type::OBJECT: return objectValue == value.objectValue;
            case Type::ARRAY: return arrayValue == value.arrayValue;
        }
    }
    return false;
}

Value& Value::operator[](const String& key) {
    return const_cast<Value&>(static_cast<const Value&>(*this)[key]);
}
const Value& Value::operator[](const String& key) const {
    const Object& object = getObjectValue();
    auto it = object.find(key);
    if (it != object.end()) {
        return it->second;
    }
    throw KeyError(key);
}

Value& Value::operator[](size_t index) {
    return const_cast<Value&>(static_cast<const Value&>(*this)[index]);
}

const Value& Value::operator[](size_t index) const {
    const Array& array = getArrayValue();
    if (index < array.size()) {
        return array[index];
    }
    throw KeyError(std::to_string(index));
}

class ValueParser : public Parser {

    Value& root;
    std::vector<Value*> stack;
    std::string key;

    void checkStack() {
        if (stack.empty()) {
            root.clear();
            stack.push_back(&root);
        }
    }

    void onNumber(double value) override {
        checkStack();
        switch (stack.back()->getType()) {
            case Type::ARRAY: stack.back()->getArrayValue().push_back(value); break;
            case Type::OBJECT: stack.back()->getObjectValue()[key] = value; break;
            default: stack.back()->setNumberValue(value); break;
        }
    }

    void onBoolean(bool value) override {
        checkStack();
        switch (stack.back()->getType()) {
            case Type::ARRAY: stack.back()->getArrayValue().push_back(value); break;
            case Type::OBJECT: stack.back()->getObjectValue()[key] = value; break;
            default: stack.back()->setBooleanValue(value); break;
        }
    }

    void onNull() override {
        checkStack();
        switch (stack.back()->getType()) {
            case Type::ARRAY: stack.back()->getArrayValue().push_back(null); break;
            case Type::OBJECT: stack.back()->getObjectValue()[key] = null; break;
            default: stack.back()->setNullValue(); break;
        }
    }

    void onString(std::string& value) override {
        checkStack();
        switch (stack.back()->getType()) {
            case Type::ARRAY: stack.back()->getArrayValue().push_back(value); break;
            case Type::OBJECT: stack.back()->getObjectValue()[key] = value; break;
            default: stack.back()->setStringValue(value); break;
        }
    }

    void onKey(std::string& key) override {
        this->key = std::move(key);
    }

    void onIndex(size_t index) override {

    }

    void onObjectStart() override {
        checkStack();
        switch (stack.back()->getType()) {
            case Type::ARRAY: {
                Array& arrayValue = stack.back()->getArrayValue();
                arrayValue.emplace_back(Object());
                stack.push_back(&arrayValue.back());
                break;
            }
            case Type::OBJECT: {
                Object& object = stack.back()->getObjectValue();
                stack.push_back(&(object[key] = Object()));
                break;
            }
            default:
                stack.back()->setObjectValue();
                break;
        }
    }

    void onObjectEnd() override {
        stack.pop_back();
    }

    void onArrayStart() override {
        checkStack();
        switch (stack.back()->getType()) {
            case Type::ARRAY: {
                Array& arrayValue = stack.back()->getArrayValue();
                arrayValue.emplace_back(Array());
                stack.push_back(&arrayValue.back());
                break;
            }
            case Type::OBJECT: {
                Object& object = stack.back()->getObjectValue();
                stack.push_back(&(object[key] = Array()));
                break;
            }
            default:
                stack.back()->setObjectValue();
                break;
        }
    }

    void onArrayEnd() override {
        stack.pop_back();
    }

public:

    ValueParser(Value& value, bool unique) : root(value), stack{&value} {
        if (!unique) {
            value.setArrayValue();
        }
    }
};

void Value::print(Printer& printer) const {
    switch (type) {
        case Type::NUMBER: printer.value(numberValue); break;
        case Type::BOOLEAN: printer.value(booleanValue); break;
        case Type::NULL_: printer.value(); break;
        case Type::STRING: printer.value(stringValue); break;
        case Type::OBJECT:
            printer.startObject();
            for (auto& key : objectValue) {
                printer.key(key.first);
                key.second.print(printer);
            }
            printer.endObject();
            break;
        case Type::ARRAY:
            printer.startArray();
            for (auto& value : arrayValue) {
                value.print(printer);
            }
            printer.endArray();
            break;
        default: throw UndefinedValueError();
    }
}

void Value::print(std::ostream& output, int indent, bool escapeUnicode, bool color) const {
    Printer printer(output, indent, escapeUnicode, color);
    print(printer);
}

void Value::parse(std::istream& input, const Path& path, bool unique) {
    clear();
    ValueParser(*this, unique).parse(input, path);
}

Value parse(const std::string& json, const Path& path, bool unique) {
    Value value;
    std::istringstream input(json);
    value.parse(input);
    return value;
}

Value load(const std::string& file, const Path& path, bool unique) {
    Value value;
    std::ifstream input(file);
    value.parse(input);
    return value;
}

const Value* Value::findFirst(Path::Cursor& cursor) const {

    if (cursor.isInTarget()) {
        return this;
    }

    if (cursor.isInPath()) {

        switch (type) {

            case Type::ARRAY:
                for (size_t index = 0; index < arrayValue.size(); index++) {
                    cursor.next(index);
                    const Value* child = arrayValue[index].findFirst(cursor);
                    if (child != nullptr) {
                        return child;
                    }
                    cursor.prev();
                }
                break;

            case Type::OBJECT:
                for (const auto& item : objectValue) {
                    cursor.next(item.first);
                    const Value* child = item.second.findFirst(cursor);
                    if (child != nullptr) {
                        return child;
                    }
                    cursor.prev();
                }
                break;
        }
    }

    return nullptr;
}

Value* Value::findFirst(const Path& path) {
    return const_cast<Value*>(static_cast<const Value&>(*this).findFirst(path));
}

const Value* Value::findFirst(const Path& path) const {
    Path::Cursor cursor(path);
    return findFirst(cursor);
}

void Value::findAll(std::vector<Value*>& all, Path::Cursor& cursor) {

    if (cursor.isInTarget()) {
        return all.push_back(this);
    }

    if (cursor.isInPath()) {

        switch (type) {

            case Type::ARRAY:
                for (size_t index = 0; index < arrayValue.size(); index++) {
                    cursor.next(index);
                    arrayValue[index].findAll(all, cursor);
                    cursor.prev();
                }
                break;

            case Type::OBJECT:
                for (auto& item : objectValue) {
                    cursor.next(item.first);
                    item.second.findAll(all, cursor);
                    cursor.prev();
                }
                break;
        }
    }
}

void Value::findAll(std::vector<const Value*>& all, Path::Cursor& cursor) const {

    if (cursor.isInTarget()) {
        return all.push_back(this);
    }

    if (cursor.isInPath()) {

        switch (type) {

            case Type::ARRAY:
                for (size_t index = 0; index < arrayValue.size(); index++) {
                    cursor.next(index);
                    arrayValue[index].findAll(all, cursor);
                    cursor.prev();
                }
                break;

            case Type::OBJECT:
                for (const auto& item : objectValue) {
                    cursor.next(item.first);
                    item.second.findAll(all, cursor);
                    cursor.prev();
                }
                break;
        }
    }
}

std::vector<Value*> Value::findAll(const Path& path) {
    Path::Cursor cursor(path);
    std::vector<Value*> all;
    findAll(all, cursor);
    return all;
}

std::vector<const Value*> Value::findAll(const Path& path) const {
    Path::Cursor cursor(path);
    std::vector<const Value*> all;
    findAll(all, cursor);
    return all;
}

}

std::ostream& operator<<(std::ostream& output, JSON::Type type) {
    switch (type) {
        case JSON::Type::NUMBER: output << "number"; break;
        case JSON::Type::BOOLEAN: output << "boolean"; break;
        case JSON::Type::NULL_: output << "null"; break;
        case JSON::Type::STRING: output << "string"; break;
        case JSON::Type::OBJECT: output << "object"; break;
        case JSON::Type::ARRAY: output << "array"; break;
        default: output << "undefined"; break;
    }
    return output;
}

std::istream& operator>>(std::istream& input, JSON::Value& value) {
    value.parse(input);
    return input;
}

std::ostream& operator<<(std::ostream& output, const JSON::Value& value) {
    value.print(output);
    return output;
}