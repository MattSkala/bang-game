#ifndef BANG_BEERCARD_H
#define BANG_BEERCARD_H


#include "InstantCard.h"


/// An instant healing card. Heals one life point to target.
class BeerCard : public InstantCard {
public:
    BeerCard(string original_name, string name, int count);
    virtual bool play(Game * game, Player * player, int position, int target);
};


#endif //BANG_BEERCARD_H
