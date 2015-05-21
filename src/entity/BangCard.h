#ifndef BANG_BANGCARD_H
#define BANG_BANGCARD_H


#include "InstantCard.h"


/// An instant offensive card. Unless a target player replies with MissCard, he loses a life point.
class BangCard : public InstantCard {
public:
    BangCard(string original_name, string name, int count);
    virtual bool play(Game * game, Player * player, int position, int target);
    virtual bool proceed(Game * game, Player * player);
};


#endif //BANG_BANGCARD_H
