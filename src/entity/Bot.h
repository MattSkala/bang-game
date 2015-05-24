#ifndef BANG_BOT_H
#define BANG_BOT_H


#include "Player.h"

class GameServer;
class Game;

/// An AI player.
class Bot : public Player {
private:
    void discardExcessCards(Game * game);

    /**
     * Generates all possible moves and computes their utility with Game::computeUtility for self and an opponent.
     * The move with largest utility for self and lowest utility for opponent is considered to be the best.
     * \return True on success, false when there is no possible move.
     */
    bool getBestMove(Game * game, int & position, int & target, int & target_card);
public:
    Bot();

    /// Bot is on turn and should play.
    void play(GameServer * server, Game * game);

    /// Bot is in pending state and can reply to pending card.
    void reply(GameServer * server, Game * game);
};


#endif //BANG_BOT_H
