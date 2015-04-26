#include <vector>
#include "Player.h"
#include "Card.h"

using namespace std;


#ifndef BANG_GAME_H
#define BANG_GAME_H


/**
 * A game state holding card pack and connected players.
 */
class Game {
private:
    vector<Card *> pack_;
    vector<Player *> players_;
    int player_on_turn_;
    Player * me_;
public:
    Game();

    /**
     * Sets current player and adds her to players list.
     */
    void setMe(Player * player);

    /**
     * Returns current player.
     */
    Player * getMe();

    /**
     * Adds a player to players list.
     */
    void addPlayer(Player * player);

    /**
     * Gets players list.
     */
    vector<Player *> & getPlayers();

    /**
     * Sets cards pack.
     */
    void setPack(vector<Card *> cards);

    /**
     * Plays a card.
     */
    bool playCard(Card * card);

    /**
     * Finishes current round and hands control over to other player.
     */
    void finishRound();
};


#endif //BANG_GAME_H
