#include "BeerCard.h"
#include "../Game.h"
#include "../net/GameServer.h"
#include <iostream>

using namespace std;

BeerCard::BeerCard(string original_name, string name, int count) : InstantCard(original_name, name, count) { }

int BeerCard::play(Game *game, Player *player, int position, int target, int target_card) {
    if (player->isPending() && (player->getLife() > 0 || target_others_)) {
        return Game::ERROR_INVALID_REACTION;
    }

    if (target_self_) {
        player->increaseLife();
    }

    if (target_others_) {
        for (Player * item : game->getPlayers()) {
            if (item != player) {
                item->increaseLife();
            }
        }
    }

    game->discardCard(player, position);

    if (player->isPending()) {
        player->setPending(false);

        if (game->getPendingPlayersCount() == 0) {
            game->setPendingCard(nullptr);
        }
    }

    return Game::SUCCESS;
}

string BeerCard::print() const {
    string str = Card::print();
    str += "  \u2665";
    return str;
}
