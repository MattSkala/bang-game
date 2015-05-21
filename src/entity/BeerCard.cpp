#include "BeerCard.h"
#include "../Game.h"
#include <iostream>

using namespace std;

BeerCard::BeerCard(string original_name, string name, int count) : InstantCard(original_name, name, count) { }

bool BeerCard::play(Game *game, Player *player, int position, int target) {
    cout << "play beer" << endl;
    if (target_self_) {
        cout << "target self" << endl;
        player->increaseLife();
    }
    if (target_others_) {
        cout << "target others" << endl;
        for (Player * item : game->getPlayers()) {
            if (item != player) {
                item->increaseLife();
            }
        }
    }
    game->discardCard(player, position);
    return true;
}
