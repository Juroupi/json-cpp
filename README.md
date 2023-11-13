# JSON in C++

A library for reading, writing, and manipulating JSON data in C++.

## Features

- Manipulate data using the `JSON::Value` class, which can represent any JSON data.
- Read any valid JSON data from a stream.
- Write data to a stream with customizable formatting.
- Find values using paths in `JSON::Value` objects or in streams.
- Read custom data structure using `JSON::Struct`.
- Syntax errors are reported with line and column numbers.
- The lexer and parser can be used independently of the rest of the library.

## Example

```cpp
#include <iostream>
#include <json/json.h>

int main() {

    JSON::Value p = JSON::parse(R"(
        {
            "name": "\u0063lodsire",
            "types": [ "poison", "ground" ],
            "height": 1.8,
            "weight": 223,
            "stats": {
                "hp": 130,
                "attack": 75,
                "defense": 60,
                "special-attack": 45,
                "special-defense": 100,
                "speed": 20
            }
        }
    )");

    std::cout << p["name"] << std::endl; // "clodsire"

    p["types"] = JSON::Array {
        JSON::String("water"),
        JSON::String("ground")
    };

    std::cout << p["types"][0] << std::endl; // "water"
    
    std::cout << *p.findFirst("$.['attack']") << std::endl; // 75
}
```

Other examples can be found in the [examples](examples) directory.

## Documentation

Documentation is in the header files.