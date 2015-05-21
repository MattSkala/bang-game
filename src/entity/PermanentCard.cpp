#include "PermanentCard.h"

PermanentCard::PermanentCard(string original_name, string name, int count) : PlayableCard(original_name, name, count) { }

bool PermanentCard::play(Game *game, Player *player, int position, int target) {
    return true;
}
