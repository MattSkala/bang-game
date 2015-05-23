#ifndef BANG_EXCEPTION_H
#define BANG_EXCEPTION_H

#include <string>

using namespace std;

/// An exception wrapper.
class Exception {
private:
    string name_;
    string message_;
public:
    Exception(string name, string message);
    string getName() const;
    string getMessage() const;
};


#endif //BANG_EXCEPTION_H
