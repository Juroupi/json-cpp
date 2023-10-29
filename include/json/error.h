#ifndef _JSON_ERROR_H_
#define _JSON_ERROR_H_

#include <string>
#include <exception>
#include <ostream>

namespace JSON {

/**
 * Base class for all JSON errors
 */
class Error : public std::exception {
protected:
    std::string message; 
public:
    const char* what() const noexcept override;
};

}

std::ostream& operator<<(std::ostream& stream, const JSON::Error& error);

#endif