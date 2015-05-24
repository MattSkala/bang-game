#include <stdlib.h>
#include "DiscardCard.h"
#include "../Game.h"
#include "../net/GameServer.h"

DiscardCard::DiscardCard(string original_name, string name, int count) : InstantCard(original_name, name, count) { }

int DiscardCard::play(Game *game, Player *player, int position, int target, int target_card) {
    if (player->isPending()) {
        return Game::ERROR_INVALID_REACTION;
    }

    Player * target_player = game->getPlayers()[target];

    if (target_player == player || !target_player->isAlive()) {
        return Game::ERROR_INVALID_TARGET;
    }

    if (target_card >= 0) {
        auto & cards = target_player->getPermanentCards();
        if (target_card >= (int) cards.size()) {
            return Game::ERROR_INVALID_TARGET_CARD;
        }
        game->discardCard(cards[target_card]);
        cards.erase(cards.begin() + target_card);
    } else {
        if (target_player->getCards().size() > 0) {
            // Discard random card from hand of target player
            target_card = rand() % (int) target_player->getCards().size();
            game->discardCard(target_player, target_card);
        } else {
            return Game::ERROR_TARGET_NO_CARDS;
        }
    }

    game->discardCard(player, position);

    return Game::SUCCESS;
}

bool DiscardCard::isCardTargetable() const {
    return true;
}

string DiscardCard::print() const {
    string str = Card::print();
    str += "  [X]";
    return str;
}
