#include "Exception.h"

Exception::Exception(string name, string message) : name_(name), message_(message) { }

string Exception::getName() const {
    return name_;
}

string Exception::getMessage() const {
    return message_;
}
