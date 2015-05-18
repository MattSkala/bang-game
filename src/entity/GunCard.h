#ifndef BANG_GUNCARD_H
#define BANG_GUNCARD_H


#include "Card.h"


/// A gun card
class GunCard : public Card {
private:
    int distance_;
    bool unlimited_bang_;
public:
    GunCard(string original_name, string name, int count);
    void setDistance(int distance);
    int getDistance() const;
    void setUnlimitedBang(bool unlimited);
    bool getUnlimitedBang() const;
};


#endif //BANG_GUNCARD_H
