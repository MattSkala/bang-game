#ifndef BANG_PLAYER_H
#define BANG_PLAYER_H


#include <string>
#include <vector>
#include <memory>
#include "RoleCard.h"
#include "CharacterCard.h"


/// A player entity storing cards on hand, a role, a character and life points.
class Player {
private:
    int life_ = 0;
    std::string name_;
    RoleCard * role_ = NULL;
    CharacterCard * character_ = NULL;
    vector<shared_ptr<Card>> cards_;
public:
    Player();

    /**
     * Sets player name.
     */
    void setName(std::string name);

    /**
     * Gets player name;
     */
    std::string getName() const;

    /**
     * Sets player role.
     */
    void setRole(RoleCard * card);

    /**
     * Gets player role.
     */
    RoleCard * getRole() const;

    /**
     * Sets player character.
     */
    void setCharacter(CharacterCard * card);

    /**
     * Gets player character.
     */
    CharacterCard * getCharacter() const;

    /**
     * Adds a card to user hand.
     */
    void addCard(shared_ptr<Card> card);

    /**
     * Sets cards in user hand.
     */
    void setCards(vector<shared_ptr<Card>> cards);

    /**
     * Gets cards in user hand.
     */
    vector<shared_ptr<Card>> & getCards();

    /**
     * Decreases life points by one.
     */
    void decreaseLife();

    /**
     * Heals one life point.
     * @return True if a life point was healed or false if user already has maximal life points count.
     */
    bool increaseLife();

    /**
     * Returns current life points count.
     */
    int getLife() const;

    /**
     * Sets current life points count.
     */
    void setLife(int life);

    /**
     * Returns maximum life points computed from role and character.
     */
    int getMaxLife() const;

    /**
     * Checks whether the player is alive (life points > 0).
     */
    bool isAlive() const;
};


#endif //BANG_PLAYER_H
