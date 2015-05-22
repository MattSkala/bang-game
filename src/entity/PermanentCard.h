#ifndef BANG_PERMANENTCARD_H
#define BANG_PERMANENTCARD_H


#include "PlayableCard.h"


/// A playable card which can be laid on table and has a permanent effect until it is removed.
class PermanentCard : public PlayableCard {
private:
    int distance_tweak_ = 0;
public:
    PermanentCard(string original_name, string name, int count);
    virtual int play(Game *game, Player *player, int position, int target, int target_card);
    int getDistanceTweak() const;
    void setDistanceTweak(int distance);
};


#endif //BANG_PERMANENTCARD_H
