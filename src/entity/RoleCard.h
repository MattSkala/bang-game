#ifndef BANG_ROLECARD_H
#define BANG_ROLECARD_H


#include "Card.h"


/// A player role card. It is assigned to each player at the start of the game.
class RoleCard : public Card {
public:
    static const string SHERIFF;
    static const string DEPUTY;
    static const string OUTLAW;
    static const string RENEGATE;
    RoleCard(string original_name, string name, int count);
};


#endif //BANG_ROLECARD_H
