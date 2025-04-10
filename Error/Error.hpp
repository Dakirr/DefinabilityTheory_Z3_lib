#ifndef ERROR_HPP
#define ERROR_HPP

#include <string>

class Error {
    public:
    std::string Type;
    std::string Message;
    Error(std::string Type, std::string Message) : Type(Type), Message(Message) {};
};

void raise_error(std::string Type, std::string Message);

#endif