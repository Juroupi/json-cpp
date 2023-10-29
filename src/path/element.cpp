#include <json/path/element.h>

namespace JSON {

Path::Element::Index::Index(size_t index) :
    index(index) {}

Path::Element::Range::Range(size_t first, size_t last, size_t step) :
    first(first), last(last), step(step) {}


bool Path::Element::accept(const std::string& name) const {
    return false;
}

bool Path::Element::accept(size_t index) const {
    return false;
}

bool Path::Element::Name::accept(const std::string& name) const {
    return *this == name;
}

bool Path::Element::Index::accept(size_t index) const {
    return this->index == index;
}

bool Path::Element::Range::accept(size_t index) const {
    return index >= first && index <= last && (index - first) % step == 0;
}

bool Path::Element::Options::accept(const std::string& name) const {
    return first->accept(name) || second->accept(name);
}

bool Path::Element::Options::accept(size_t index) const {
    return first->accept(index) || second->accept(index);
}

bool Path::Element::Any::accept(const std::string& name) const {
    return true;
}

bool Path::Element::Any::accept(size_t index) const {
    return true;
}

bool Path::Element::Function::accept(const std::string& name) const {
    return first(name);
}

bool Path::Element::Function::accept(size_t index) const {
    return second(index);
}


void JSON::Path::Element::Name::print(std::ostream& output) const {
    output << '\"' << *this << '\"';
}

void JSON::Path::Element::Index::print(std::ostream& output) const {
    output << index;
}

void JSON::Path::Element::Any::print(std::ostream& output) const {
    output << '*';
}

void JSON::Path::Element::Function::print(std::ostream& output) const {
    output << "f(@)";
}

void JSON::Path::Element::Range::print(std::ostream& output) const {
    output << first << ':' << last;
    if (step != 1) {
        output << ':' << step;
    }
}

void JSON::Path::Element::Options::print(std::ostream& output) const {
    first->print(output);
    output << ", ";
    second->print(output);
}

}