//
// Created by Matouš Skála on 26.04.15.
//

#ifndef BANG_CONTROLLER_H
#define BANG_CONTROLLER_H


/**
 * An abstract controller.
 */
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
