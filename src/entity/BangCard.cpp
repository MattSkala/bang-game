#include "BangCard.h"
#include "../Game.h"

BangCard::BangCard(string original_name, string name, int count) : InstantCard(original_name, name, count) { }

bool BangCard::play(Game *game, Player *player, int position, int target) {
    if (player->hasPlayedBang() && !player->hasUnlimitedBang()) {
        return false;
    }

    int distance = game->getDistance(player, target);
    int range = player->getGunRange();

    // Player is too far
    if (distance > range) {
        return false;
    }

    Player * target_player = game->getPlayers()[target];
    game->setPendingCard(this);
    target_player->setPending(true);
    game->discardCard(player, position);

    player->setPlayedBang(true);

    return true;
}

bool BangCard::proceed(Game *game, Player *player) {
    player->decreaseLife();
    return true;
}
