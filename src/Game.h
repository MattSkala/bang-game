#ifndef BANG_GAME_H
#define BANG_GAME_H

#include <vector>
#include <memory>
#include <list>
#include "entity/Player.h"
#include "entity/Card.h"
#include "entity/CharacterCard.h"
#include "entity/RoleCard.h"


/// A game engine holding card pack, connected players and current game state.
class Game {
private:
    vector<shared_ptr<RoleCard>> roles_;
    vector<shared_ptr<CharacterCard>> characters_;
    vector<shared_ptr<Card>> cards_;
    list<shared_ptr<Card>> pack_;
    vector<Player *> players_;
    Player * player_on_turn_;
    Player * me_;

    /**
     * Assigns roles to players.
     */
    void assignRoles();

    /**
     * Assigns characters to players.
     */
    void assignCharacters();

    /**
     * Assigns cards to players.
     */
    void assignCards();

    /**
     * Sets players default life points to max value.
     */
    void setLifePoints();

    /**
     * Finds a player by name.
     */
    Player * getPlayer(string name);

    /**
     * Finds a card by name.
     */
    shared_ptr<Card> getCard(string name);
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
     * Initializes the game. Should be called by server.
     */
    void initGame();

    /**
     * Finds cards in pack by their original names.
     */
    vector<shared_ptr<Card>> getCardsByNames(vector<string> names) const;

    /**
     * Gets player on turn.
     */
    Player * getPlayerOnTurn();

    /**
     * Updates player info. Should be called by client when received updated info from server.
     */
    bool updatePlayer(string name, int life, string character, string role, bool on_turn);

    /**
     * A player draws a card.
     */
    void drawCard(Player * player);

    /**
     * Draws two cards at the beginning phase of the round.
     * \return True on success, false if player cannot draw cards.
     */
    bool drawCards(Player * player);

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
