#ifndef BANG_BEERCARD_H
#define BANG_BEERCARD_H


#include "InstantCard.h"


/// An instant healing card. Heals one life point to target.
class BeerCard : public InstantCard {
public:
    BeerCard(string original_name, string name, int count);
    virtual int play(Game * game, Player * player, int position, int target, int target_card);
    virtual string print() const;
};


#endif //BANG_BEERCARD_H
