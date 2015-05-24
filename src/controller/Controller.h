#ifndef BANG_CONTROLLER_H
#define BANG_CONTROLLER_H

#include <string>

using namespace std;

/// An abstract controller.
class Controller {
protected:
    /// Clears console screen.
    void clearScreen();

    /// Prints game logo in ASCII art.
    void printLogo();

    /**
     * \brief Scans integer from input string.
     * \return True on success, false if input validation failed.
     */
    bool scanInt(string in, int & res);

    /**
     * \brief Scans char from input string.
     * \return True on success, false if input validation failed.
     */
    bool scanChar(string in, char & res);
public:
    virtual ~Controller();
};


#endif //BANG_CONTROLLER_H
