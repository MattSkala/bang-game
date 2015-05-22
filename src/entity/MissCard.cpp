#include "MissCard.h"
#include "../net/GameServer.h"

MissCard::MissCard(string original_name, string name, int count) : InstantCard(original_name, name, count) { }

int MissCard::play(Game *game, Player *player, int position, int target, int target_card) {
    if (!player->isPending()) {
        return Game::ERROR_INVALID_MISS;
    }

    player->setPending(false);

    if (game->getPendingPlayersCount() == 0) {
        game->setPendingCard(nullptr);
    }

    game->discardCard(player, position);

    return Game::SUCCESS;
}
