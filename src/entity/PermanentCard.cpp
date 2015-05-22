#include "PermanentCard.h"
#include "Player.h"
#include "../net/GameServer.h"

PermanentCard::PermanentCard(string original_name, string name, int count) : PlayableCard(original_name, name, count) { }

int PermanentCard::play(Game *game, Player *player, int position, int target, int target_card) {
    if (player->isPending()) {
        return Game::ERROR_INVALID_REACTION;
    }

    // Check if this card is already laid
    for (auto & card : player->getPermanentCards()) {
        if (card->getOriginalName() == getOriginalName()) {
            return Game::ERROR_CARD_ALREADY_LAID;
        }
    }

    player->layCard(position);
    return Game::SUCCESS;
}

int PermanentCard::getDistanceTweak() const {
    return distance_tweak_;
}

void PermanentCard::setDistanceTweak(int distance) {
    distance_tweak_ = distance;
}
