#ifndef BANG_DRAWCARD_H
#define BANG_DRAWCARD_H


#include "InstantCard.h"


/// An instant card. A player draws a predefined number of cards.
class DrawCard : public InstantCard {
private:
    int draw_count_;
public:
    DrawCard(string original_name, string name, int count, int draw_count);
    virtual int play(Game * game, Player * player, int position, int target, int target_card);
    virtual string print() const;
    virtual bool isCardTargetable() const;
};


#endif //BANG_DRAWCARD_H
