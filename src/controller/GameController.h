#ifndef BANG_GAMECONTROLLER_H
#define BANG_GAMECONTROLLER_H

#include "Controller.h"
#include "../Game.h"
#include "../net/GameClient.h"

/// A controller for game screen.
class GameController : public Controller {
    Game & game_;
    GameClient & client_;
    function<bool(vector<string>)> listener_;
    bool on_turn_ = false;
    bool onStreamEvent(vector<string> event);

    void updatePlayersInfo();
public:
    GameController(Game & game, GameClient & client);

    /**
     * Fetches card in hand and other players characters.
     */
    void actionInit();

    /**
     * Fetches current game state from server.
     */
    void actionRefresh();

    /**
     * Renders game board with players list and cards.
     */
    void renderBoard();
};


#endif //BANG_GAMECONTROLLER_H
