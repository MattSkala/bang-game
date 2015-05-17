#ifndef BANG_GAME_H
#define BANG_GAME_H

#include <vector>
#include <memory>
#include "entity/Player.h"
#include "entity/Card.h"
#include "entity/CharacterCard.h"
#include "entity/RoleCard.h"


/**
 * A game state holding card pack and connected players.
 */
class Game {
private:
    vector<shared_ptr<RoleCard>> roles_;
    vector<shared_ptr<CharacterCard>> characters_;
    vector<shared_ptr<Card>> pack_;
    vector<Player *> players_;
    int player_on_turn_;
    Player * me_;
public:
    Game();

    ~Game();

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
