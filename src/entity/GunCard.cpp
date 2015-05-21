#include <vector>
#include "GunCard.h"
#include "Player.h"

GunCard::GunCard(string original_name, string name, int count, int distance) : PermanentCard(original_name, name, count), distance_(distance) { }

bool GunCard::play(Game *game, Player *player, int position, int target) {
    // Remove existing gun
    vector<shared_ptr<PermanentCard>> cards = player->getPermanentCards();
    for (unsigned int i = 0; i < cards.size(); i++) {
        if (dynamic_cast<GunCard *>(cards[i].get())) {
            cards.erase(cards.begin() + i);
        }
    }

    player->layCard(position);
    return true;
}

int GunCard::getDistance() const {
    return distance_;
}

void GunCard::setUnlimitedBang(bool unlimited) {
    unlimited_bang_ = unlimited;
}

bool GunCard::hasUnlimitedBang() const {
    return unlimited_bang_;
}
