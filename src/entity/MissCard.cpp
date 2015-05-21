#include "MissCard.h"

MissCard::MissCard(string original_name, string name, int count) : InstantCard(original_name, name, count) { }

bool MissCard::play(Game *game, Player *player, int position, int target) {
    player->setPending(false);
    if (game->getPendingPlayersCount() == 0) {
        game->setPendingCard(nullptr);
    }
    game->discardCard(player, position);
    return true;
}
