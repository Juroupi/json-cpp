#include <JSON/path.h>
#include <JSON/path/lexer.h>
#include <JSON/path/parser.h>
#include <JSON/path/element.h>

namespace JSON {

Path::Path(const std::string& path) {
    Parser(*this, path).parse();
}

Path::Path(const char* path) :
    Path(std::string(path)) {}

size_t Path::getSize() const {
    return elements.size();
}

const Path::Element& Path::operator[](size_t index) const {
    return *elements[index];
}

Path& Path::operator<<(DeepScan) {
    deepScanNext = true;
    return *this;
}

Path& Path::operator<<(std::unique_ptr<Element>&& element) {
    element->deepScan = deepScanNext;
    deepScanNext = false;
    elements.emplace_back(std::move(element));
    return *this;
}

}

std::ostream& operator<<(std::ostream& output, const JSON::Path& path) {
    output << '$';
    for (size_t i = 0; i < path.getSize(); i++) {
        if (path[i].deepScan) {
            output << '.';
        }
        output << '[';
        path[i].print(output);
        output << ']';
    }
    return output;
}