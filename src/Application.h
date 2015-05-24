#ifndef BANG_BOOTSTRAP_H
#define BANG_BOOTSTRAP_H


#include "Game.h"
#include "net/GameClient.h"
#include "unistd.h"

/// An application instance that handles user flow and stores app state.
class Application {
private:
    Game game_;
    GameClient client_;
    pid_t server_pid_ = 0;
public:
    ~Application();

    /// Initializes the application and shows welcome screen.
    void init(string filename);
};


#endif //BANG_BOOTSTRAP_H
