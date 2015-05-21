#ifndef BANG_PERMANENTCARD_H
#define BANG_PERMANENTCARD_H


#include "PlayableCard.h"


/// A playable card which can be laid on table and has a permanent effect until it is removed.
class PermanentCard : public PlayableCard {
public:
    PermanentCard(string original_name, string name, int count);
    virtual bool play(Game * game, Player * player, int position, int target);
};


#endif //BANG_PERMANENTCARD_H
