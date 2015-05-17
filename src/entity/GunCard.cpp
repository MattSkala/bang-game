#include "GunCard.h"

GunCard::GunCard(string original_name, string name, int count) : Card(original_name, name, count) { }

void GunCard::setDistance(int distance) {
    distance_ = distance;
}

int GunCard::getDistance() const {
    return distance_;
}

void GunCard::setUnlimitedBang(bool unlimited) {
    unlimited_bang_ = unlimited;
}

bool GunCard::getUnlimitedBang() const {
    return unlimited_bang_;
}
