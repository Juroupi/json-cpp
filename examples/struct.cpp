#include <iostream>
#include <sstream>
#include <cstddef>
#include <json/struct.h>

struct B {
    int b1;
    float b2;
};

struct A {
    bool a1;
    std::string a2;
    std::vector<int> a3;
    int a4;
    B sub;
};

int main() {

    std::string str = R"({
        "a1": true,
        "sub": {
            "b2": 56,
            "b1": 34
        },
        "a2": "wow",
        "a3": [ 10, 9, 8 ]
    })";

    JSON::Struct bStruct {
        { "['b1']", JSON::Struct::OFFSET<offsetof(B, b1)>, JSON::Struct::NUMBER<int> },
        { "['b2']", JSON::Struct::OFFSET<offsetof(B, b2)>, JSON::Struct::NUMBER<float> }
    };

    JSON::Struct aStruct {
        { "['a1']", JSON::Struct::OFFSET<offsetof(A, a1)>, JSON::Struct::BOOLEAN },
        { "['a2']", JSON::Struct::OFFSET<offsetof(A, a2)>, JSON::Struct::STRING },
        { "['a3']", JSON::Struct::VECTOR_APPEND<int, JSON::Struct::OFFSET<offsetof(A, a3)>>, JSON::Struct::NUMBER<int> },
        { "['a4']", JSON::Struct::OFFSET<offsetof(A, a4)>, JSON::Struct::NUMBER<int>, JSON::Struct::DEFAULT<int, 7> },
        { "['sub']", JSON::Struct::OFFSET<offsetof(A, sub)>, bStruct }
    };

    auto input = std::istringstream(str);

    A a;
    aStruct.parse(&a, input);

    std::cout << "a.a1: " << (a.a1 ? "true" : "false") << std::endl;
    std::cout << "a.a2: \"" << a.a2 << "\"" << std::endl;
    std::cout << "a.a3: ";
    for (int i : a.a3) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "a.a4: " << a.a4 << std::endl;
    std::cout << "a.sub.b1: " << a.sub.b1 << std::endl;
    std::cout << "a.sub.b2: " << a.sub.b2 << std::endl;

    return 0;
}