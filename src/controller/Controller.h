#ifndef BANG_CONTROLLER_H
#define BANG_CONTROLLER_H


/// An abstract controller.
class Controller {
protected:
    /**
     * Clears console screen.
     */
    void clearScreen();

    /**
     * Prints game logo in ASCII art.
     */
    void printLogo();
};


#endif //BANG_CONTROLLER_H
