#ifndef BANG_GAMECONTROLLER_H
#define BANG_GAMECONTROLLER_H

#include "Controller.h"
#include "../Game.h"
#include "../net/GameClient.h"
#include "../net/GameServer.h"

/// A controller for game screen.
class GameController : public Controller {
    /**
     * Another player is playing, we are waiting...
     */
    static const int STATE_WAIT = 0;
    /**
     * We are on turn. What shall we do?
     */
    static const int STATE_ON_TURN = 1;
    /**
     * We are about to play a card.
     */
    static const int STATE_PLAY_CARD = 2;
    /**
     * We are selecting target for previously selected card.
     */
    static const int STATE_PLAY_TARGET = 3;
    /**
     * We are selecting target laid card position at previously selected player.
     */
    static const int STATE_PLAY_TARGET_CARD = 4;
    /**
     * We are selecting card to discard.
     */
    static const int STATE_DISCARD_CARD = 5;
    /**
     * We have to reply to pending card.
     */
    static const int STATE_PENDING = 6;
    /**
     * The game has ended.
     */
    static const int STATE_GAME_OVER = 7;

    Game & game_;
    GameClient & client_;
    function<bool(vector<string>)> listener_;
    bool onStreamEvent(vector<string> event);
    int state_ = GameController::STATE_WAIT;
    int error_ = Game::SUCCESS;
    vector<string> winners_;

    shared_ptr<PlayableCard> last_card_;
    string last_card_player_;
    int last_card_target_;

    void update();
    void updatePlayersInfo();
    void updatePermanentCards();
    void updateCards();
    void playCard(int position, int target = -1, int target_card = -1);
    void initInputHandler();
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
