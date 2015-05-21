#include "PermanentCard.h"
#include "Player.h"

PermanentCard::PermanentCard(string original_name, string name, int count) : PlayableCard(original_name, name, count) { }

bool PermanentCard::play(Game *game, Player *player, int position, int target) {
    // Check if this card is already laid
    for (auto & card : player->getPermanentCards()) {
        if (card->getOriginalName() == getOriginalName()) {
            return false;
        }
    }

    player->layCard(position);
    return true;
}

int PermanentCard::getDistanceTweak() const {
    return distance_tweak_;
}

void PermanentCard::setDistanceTweak(int distance) {
    distance_tweak_ = distance;
}
