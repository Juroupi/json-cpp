#ifndef _JSON_STRUCT_H_
#define _JSON_STRUCT_H_

#include <json/type.h>
#include <json/parser.h>
#include <json/path.h>
#include <vector>
#include <istream>
#include <cstddef>

namespace JSON {

class Struct {

    void parse(void* base, Parser& parser);

public:

    /**
     * A function that sets a primitive value for a field in a struct.
     * The base pointer is the pointer to the structure to be filled.
     * The offset is the offset of the field in the struct (obtained with offsetof).
     * The count is the number of times the field has been found.
     * The type is the type of the value : NUMBER, BOOLEAN, STRING or NULL_.
     * The value is the value to set : double* for NUMBER, bool* for BOOLEAN, std::string* for STRING and nullptr for NULL_.
     */
    using PrimitiveSetter = bool (*)(void* base, size_t offset, int count, Type type, void* value);

    /**
     * A function that sets a default value for a field in a struct.
     */
    using DefaultSetter = void (*)(void* base, size_t offset);

    /**
     * A struct field.
     * It can be a primitive type (int, float, bool, ...) or a struct.
     */
    struct Field {

        /**
         * The path to the field.
         */
        Path path;

        /**
         * The offset of the field in the struct.
         * Can be obtained with offsetof.
         */
        size_t offset;

        /**
         * The setter for the field if it is a primitive type.
         */
        PrimitiveSetter setPrimitive;

        /**
         * The sub-struct if the field is a struct.
         */
        Struct* subStruct;

        /**
         * The default setter for the field.
         * It is called if the field has not been found.
         */
        DefaultSetter setDefault;

        /**
         * Constructs a primitive field.
         */
        Field(Path path, size_t offset, PrimitiveSetter setPrimitive, DefaultSetter setDefault = nullptr);

        /**
         * Constructs a struct field.
         */
        Field(Path path, size_t offset, Struct& subStruct, DefaultSetter setDefault = nullptr);
    };
    
    /**
     * Some setters for char, std::string and bool.
     */
    static const PrimitiveSetter CHAR, STRING, BOOLEAN;

    /**
     * A setter for numbers (int, long, float, double, ...).
     */
    template<typename NumberType>
    static bool NUMBER(void* base, size_t offset, int count, Type type, void* value) {
        if (type == Type::NUMBER) {
            *(NumberType*)((unsigned char*)base + offset) = (NumberType)*(double*)value;
            return true;
        }
        return false;
    }

    /**
     * A default setter that makes the field mandatory by raising an exception when called.
     */
    static const DefaultSetter MANDATORY;

    /**
     * A simple default setter that sets the field to a default value.
     */
    template<typename Type, Type defaultValue>
    static void DEFAULT(void* base, size_t offset) {
        *(Type*)((unsigned char*)base + offset) = defaultValue;
    }

protected:

    std::vector<Field> fields;

public:

    /**
     * Constructs a struct with the given fields.
     */
    Struct(std::initializer_list<Field> fields);

    /**
     * Returns the fields of the struct.
     */
    const std::vector<Field>& getFields() const;

    /**
     * Parses the struct from the given input.
     * The base pointer is the pointer to the structure to be filled.
     */
    void parse(void* base, std::istream& input, const Path& path = {});
};

}

#endif