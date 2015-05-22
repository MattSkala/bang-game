#ifndef BANG_DISCARDCARD_H
#define BANG_DISCARDCARD_H


#include "InstantCard.h"


/// An instant card. A player draws a predefined number of cards.
class DiscardCard : public InstantCard {
public:
    DiscardCard(string original_name, string name, int count);
    virtual int play(Game * game, Player * player, int position, int target, int target_card);
    virtual bool isCardTargetable() const;
};


#endif //BANG_DISCARDCARD_H
