#include <json/struct.h>
#include <json/parser.h>
#include <json/path/cursor.h>
#include <string>

namespace JSON {

struct StructFieldInfo {

    const Struct::Field& field;
    Path::Cursor cursor;
    int count;

    StructFieldInfo(const Struct::Field& field) :
        field(field), cursor(field.path), count(0) {}
};

struct StructFieldInfos : std::vector<StructFieldInfo> {

    StructFieldInfos(const std::vector<Struct::Field>& fields) {
        reserve(fields.size());
        for (const Struct::Field& field : fields) {
            emplace_back(field);
        }
    }

    void setDefaults(void* base) {
        for (StructFieldInfo& fieldInfo : *this) {
            if (fieldInfo.count == 0 && fieldInfo.field.setDefault != nullptr) {
                fieldInfo.field.setDefault(base, fieldInfo.field.offset);
            }
        }
    }
};


class StructParser : public Parser {

    void* base;
    StructFieldInfos& fieldInfos;

    void trySetPrimitive(Type type, void* value) {
        for (StructFieldInfo& fieldInfo : fieldInfos) {
            if (fieldInfo.field.setPrimitive != nullptr && fieldInfo.cursor.isInTarget()) {
                if ((*fieldInfo.field.setPrimitive)(base, fieldInfo.field.offset, fieldInfo.count, type, value)) {
                    fieldInfo.count++;
                    return;
                }
            }
        }
    }

    void delegateStruct(size_t offset, Struct& subStruct) {
        void* subBase = (unsigned char*)base + offset;
        StructFieldInfos subFieldInfos(subStruct.getFields());
        StructParser subParser(subBase, subFieldInfos);
        Parser::delegate(subParser);
        subFieldInfos.setDefaults(subBase);
    }

    void trySetStruct() {
        for (StructFieldInfo& fieldInfo : fieldInfos) {
            if (fieldInfo.field.subStruct != nullptr && fieldInfo.cursor.isInTargetRoot()) {
                delegateStruct(fieldInfo.field.offset, *fieldInfo.field.subStruct);
                fieldInfo.count++;
                prev();
                return;
            }
        }
    }

    void prev() {
        if (getDepth() > 0) {
            for (StructFieldInfo& fieldInfo : fieldInfos) {
                fieldInfo.cursor.prev();
            }
        }
    }

public:

    void onObjectStart() override {}
    void onObjectEnd() override { prev(); }
    void onArrayStart() override {}
    void onArrayEnd() override { prev(); }

    void onKey(std::string& key) override {
        for (StructFieldInfo& fieldInfo : fieldInfos) {
            fieldInfo.cursor.next(key);
        }
        trySetStruct();
    }

    void onIndex(size_t index) override {
        for (StructFieldInfo& fieldInfo : fieldInfos) {
            fieldInfo.cursor.next(index);
        }
        trySetStruct();
    }

    void onNumber(double value) override {
        trySetPrimitive(Type::NUMBER, &value);
        prev();
    }

    void onBoolean(bool value) override {
        trySetPrimitive(Type::BOOLEAN, &value);
        prev();
    }

    void onString(std::string& value) override {
        trySetPrimitive(Type::STRING, &value);
        prev();
    }

    void onNull() override {
        trySetPrimitive(Type::NULL_, nullptr);
        prev();
    }

    StructParser(void* base, StructFieldInfos& fieldInfos) :
        base(base), fieldInfos(fieldInfos) {}
};


Struct::Field::Field(Path path, size_t offset, PrimitiveSetter setPrimitive, DefaultSetter setDefault) :
    path(std::move(path)), offset(offset), setPrimitive(setPrimitive), subStruct(nullptr), setDefault(setDefault) {}

Struct::Field::Field(Path path, size_t offset, Struct& subStruct, DefaultSetter setDefault) :
    path(std::move(path)), offset(offset), setPrimitive(nullptr), subStruct(&subStruct), setDefault(setDefault) {}


Struct::Struct(std::initializer_list<Field> fields) :
    fields(fields) {}

const std::vector<Struct::Field>& Struct::getFields() const {
    return fields;
}

void Struct::parse(void* base, std::istream& input, const Path& path) {
    StructFieldInfos fieldInfos(fields);
    StructParser structParser(base, fieldInfos);
    structParser.parse(input, path);
    fieldInfos.setDefaults(base);
}

static bool setCharPrimitive(void* base, size_t offset, int count, Type type, void* value) {
    if (type == Type::STRING) {
        std::string& stringValue = *(std::string*)value;
        if (stringValue.length() == 1) {
            ((char*)base)[offset] = stringValue[0];
            return true;
        }
    }
    return false;
}

static bool setStringPrimitive(void* base, size_t offset, int count, Type type, void* value) {
    if (type == Type::STRING) {
        *(std::string*)((unsigned char*)base + offset) = *(std::string*)value;
        return true;
    }
    return false;
}

static bool setBooleanPrimitive(void* base, size_t offset, int count, Type type, void* value) {
    if (type == Type::BOOLEAN) {
        *(bool*)((unsigned char*)base + offset) = *(bool*)value;
        return true;
    }
    return false;
}

const Struct::PrimitiveSetter Struct::CHAR = &setCharPrimitive;
const Struct::PrimitiveSetter Struct::STRING = &setStringPrimitive;
const Struct::PrimitiveSetter Struct::BOOLEAN = &setBooleanPrimitive;

static void setMandatoryDefault(void* base, size_t offset) {
    throw std::runtime_error("Mandatory field not found");
}

const Struct::DefaultSetter Struct::MANDATORY = &setMandatoryDefault;

}