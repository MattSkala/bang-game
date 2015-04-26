#ifndef BANG_BOOTSTRAP_H
#define BANG_BOOTSTRAP_H


#include "Game.h"

/**
 * An application instance that handles user flow.
 * @author Matouš Skála
 */
class Application {
private:
    Game game_;
public:
    /**
     * Initializes the application and shows welcome screen.
     */
    void init();
};


#endif //BANG_BOOTSTRAP_H
