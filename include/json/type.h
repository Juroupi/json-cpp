#ifndef _JSON_TYPE_H_
#define _JSON_TYPE_H_

namespace JSON {

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

}

#endif