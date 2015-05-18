#ifndef BANG_CHARACTERCARD_H
#define BANG_CHARACTERCARD_H


#include "Card.h"


/// A player character card. It is assigned to each player at the start of the game.
class CharacterCard : public Card {
private:
    int life_;
public:
    CharacterCard(string original_name, string name, int count);
    void setLife(int life);
    int getLife() const;
};


#endif //BANG_CHARACTERCARD_H
