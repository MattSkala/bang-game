#ifndef BANG_BOOTSTRAPCONTROLLER_H
#define BANG_BOOTSTRAPCONTROLLER_H


#include "Controller.h"
#include "../Game.h"

/**
 * A controller for welcome screen, creating a game and connecting players to it.
 */
class BootstrapController : public Controller {
private:
    Game & game_;
public:
    BootstrapController(Game & game);
    void renderWelcome();
    void renderNameInput();
    void renderPlayersList();
    void actionHostGame();
    void actionAddBot();
    void actionJoinGame();
    void actionStartGame();
};


#endif //BANG_BOOTSTRAPCONTROLLER_H
