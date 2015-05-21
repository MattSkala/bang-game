#include "PlayableCard.h"

PlayableCard::PlayableCard(string original_name, string name, int count) : Card(original_name, name, count) { }

void PlayableCard::setTargetAny(bool target) {
    target_any_ = target;
}

void PlayableCard::setTargetOthers(bool target) {
    target_others_ = target;
}

void PlayableCard::setTargetDistance(bool target) {
    target_distance_ = target;
}

void PlayableCard::setTargetSelf(bool target) {
    target_self_ = target;
}

bool PlayableCard::isTargetable() {
    return target_any_ || target_distance_;
}

bool PlayableCard::proceed(Game *game, Player *player) {
    return false;
}
