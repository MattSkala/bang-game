#include <vector>
#include "GunCard.h"
#include "Player.h"
#include "../net/GameServer.h"

GunCard::GunCard(string original_name, string name, int count, int distance) : PermanentCard(original_name, name, count), distance_(distance) { }

int GunCard::play(Game *game, Player *player, int position, int target, int target_card) {
    if (player->isPending()) {
        return Game::ERROR_INVALID_REACTION;
    }

    // Remove existing gun
    auto & cards = player->getPermanentCards();
    for (unsigned int i = 0; i < cards.size(); i++) {
        if (dynamic_cast<GunCard *>(cards[i].get())) {
            game->discardCard(cards[i]);
            cards.erase(cards.begin() + i);
        }
    }

    player->layCard(position);
    return Game::SUCCESS;
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

string GunCard::print() const {
    string str = Card::print();
    str += "  \u2205 " + to_string(distance_);
    if (hasUnlimitedBang()) {
        str += "  \u221E";
    }
    return str;
}
