#include "BangCard.h"
#include "../Game.h"
#include "../net/GameServer.h"

BangCard::BangCard(string original_name, string name, int count) : InstantCard(original_name, name, count) { }

int BangCard::play(Game *game, Player *player, int position, int target, int target_card) {
    if (player->isPending()) {
        return Game::ERROR_INVALID_REACTION;
    }

    if (target_distance_) {
        Player *target_player = game->getPlayers()[target];
        if (target_player == player || !target_player->isAlive()) {
            return Game::ERROR_INVALID_TARGET;
        }

        // Bang!
        if (player->hasPlayedBang() && !player->hasUnlimitedBang()) {
            return Game::ERROR_BANG_LIMIT;
        }

        int distance = game->getDistance(player, target);
        int range = player->getGunRange();

        // Player is too far
        if (distance > range) {
            return Game::ERROR_OUT_OF_RANGE;
        }

        game->setPendingCard(this);
        target_player->setPending(true);
        game->discardCard(player, position);

        player->setPlayedBang(true);
    } else if (target_others_) {
        // Gatling
        game->setPendingCard(this);
        for (Player *p : game->getPlayers()) {
            if (p != player && p->isAlive()) {
                p->setPending(true);
            }
        }
        game->discardCard(player, position);
    } else {
        return Game::ERROR_UNKNOWN_CARD;
    }

    return Game::SUCCESS;
}

bool BangCard::proceed(Game *game, Player *player) {
    player->decreaseLife();
    return true;
}
