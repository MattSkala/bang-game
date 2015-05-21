#include "BangCard.h"
#include "../Game.h"

const string BangCard::ORIGINAL_NAME = "Bang!";

BangCard::BangCard(string original_name, string name, int count) : InstantCard(original_name, name, count) { }

bool BangCard::play(Game *game, Player *player, int position, int target) {
    Player * target_player = game->getPlayers()[target];
    game->setPendingCard(this);
    target_player->setPending(true);
    game->discardCard(player, position);
    return true;
}

bool BangCard::proceed(Game *game, Player *player) {
    player->decreaseLife();
    return true;
}
