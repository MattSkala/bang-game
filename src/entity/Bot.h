#ifndef BANG_BOT_H
#define BANG_BOT_H


#include "Player.h"

class GameServer;
class Game;

class Bot : public Player {
private:
    void discardExcessCards(Game * game);
    bool getBestMove(Game * game, int & position, int & target, int & target_card);
public:
    Bot();

    /**
     * Bot is on turn and should play.
     */
    void play(GameServer * server, Game * game);

    /**
     * Bot is in pending state and can reply to pending card.
     */
    void reply(GameServer * server, Game * game);
};


#endif //BANG_BOT_H
