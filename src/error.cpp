#include <json/error.h>

namespace JSON {

const char* Error::what() const noexcept {
    return message.c_str();
}

}

std::ostream& operator<<(std::ostream& stream, const JSON::Error& error) {
    return stream << error.what();
}