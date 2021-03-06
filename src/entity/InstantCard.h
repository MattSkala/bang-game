#ifndef BANG_INSTANTCARD_H
#define BANG_INSTANTCARD_H


#include "PlayableCard.h"


/// A playable card which can be played only once and has an instant effect
class InstantCard : public PlayableCard {
public:
    InstantCard(string original_name, string name, int count);
};


#endif //BANG_INSTANTCARD_H
