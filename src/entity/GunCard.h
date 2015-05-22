#ifndef BANG_GUNCARD_H
#define BANG_GUNCARD_H


#include "PermanentCard.h"


/// A gun card
class GunCard : public PermanentCard {
private:
    int distance_;
    bool unlimited_bang_;
public:
    GunCard(string original_name, string name, int count, int distance);
    virtual int play(Game * game, Player * player, int position, int target, int target_card);
    int getDistance() const;
    void setUnlimitedBang(bool unlimited);
    bool hasUnlimitedBang() const;
    virtual string print() const;
};


#endif //BANG_GUNCARD_H
