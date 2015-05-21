#include "InstantCard.h"

InstantCard::InstantCard(string original_name, string name, int count) : PlayableCard(original_name, name, count) { }

bool InstantCard::play(Game *game, Player *player, int position, int target) {
    return true;
}
