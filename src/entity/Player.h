#ifndef BANG_PLAYER_H
#define BANG_PLAYER_H


#include <string>
#include <vector>
#include <memory>
#include "RoleCard.h"
#include "CharacterCard.h"
#include "PlayableCard.h"
#include "PermanentCard.h"


/// A player entity storing cards on hand, a role, a character and life points.
class Player {
private:
    unsigned int life_ = 0;
    std::string name_;
    RoleCard * role_ = NULL;
    CharacterCard * character_ = NULL;
    int cards_count_;

    /// Cards in hand.
    vector<shared_ptr<PlayableCard>> cards_;

    /// Cards laid on table.
    vector<shared_ptr<PermanentCard>> permanents_;

    /// A player should reply to pending card.
    bool pending_ = false;

    /// A player already played bang in this round.
    bool played_bang_ = false;
public:
    Player();

    virtual ~Player();

    /// Sets player name.
    void setName(std::string name);

    /// Gets player name;
    std::string getName() const;

    /// Sets player role.
    void setRole(RoleCard * card);

    /// Gets player role.
    RoleCard * getRole() const;

    /// Sets player character.
    void setCharacter(CharacterCard * card);

    /// Gets player character.
    CharacterCard * getCharacter() const;

    /// Adds a card to user hand.
    void addCard(shared_ptr<PlayableCard> card);

    /// Sets cards in user hand.
    void setCards(vector<shared_ptr<PlayableCard>> cards);

    /// Sets cards in hand count. Should be called by client on users who have invisible cards.
    void setCardsCount(int count);

    /// Gets cards in hand count. Should be called by client on users who have invisible cards.
    int getCardsCount() const;

    /// Lays card from hand to table.
    void layCard(int position);

    /// Gets cards in user hand.
    vector<shared_ptr<PlayableCard>> & getCards();

    /// Checks whether the player has beer card in hand.
    bool hasBeerCard() const;

    /// Sets permanent cards.
    void setPermanentCards(vector<shared_ptr<PermanentCard>> cards);

    /// Gets laid permanent cards.
    vector<shared_ptr<PermanentCard>> & getPermanentCards();

    /// Gets shooting range.
    int getGunRange() const;

    /// Checks whether the player can play unlimited number of BangCard in a single round.
    bool hasUnlimitedBang() const;

    /// Gets from distance tweak caused by permanent cards.
    int getDistanceFrom() const;

    /// Gets to distance tweak caused by permanent cards.
    int getDistanceTo() const;

    /// Decreases life points by one.
    void decreaseLife();

    /**
     * \brief Heals one life point.
     * @return True if a life point was healed or false if user already has maximal life points count.
     */
    bool increaseLife();

    /// Returns current life points count.
    unsigned int getLife() const;

    /// Sets current life points count.
    void setLife(unsigned int life);

    /// Returns maximum life points computed from role and character.
    unsigned int getMaxLife() const;

    /// Checks whether the player is alive (life points > 0).
    bool isAlive() const;

    void setPending(bool pending);

    bool isPending() const;

    void setPlayedBang(bool played);

    bool hasPlayedBang() const;
};


#endif //BANG_PLAYER_H
