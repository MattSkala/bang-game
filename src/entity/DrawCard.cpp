#include "DrawCard.h"
#include "../Game.h"

DrawCard::DrawCard(string original_name, string name, int count, int draw_count)
        : InstantCard(original_name, name, count), draw_count_(draw_count) { }

bool DrawCard::play(Game *game, Player *player, int position, int target) {
    for (int i = 0; i < draw_count_; i++) {
        game->drawCard(player);
    }
    game->discardCard(player, position);
    return true;
}
