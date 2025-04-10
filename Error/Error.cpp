#include "Error.hpp"

void raise_error(std::string Type, std::string Message) {
    throw Error(Type, Message);
};
