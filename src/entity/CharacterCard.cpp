#include "CharacterCard.h"

CharacterCard::CharacterCard(string original_name, string name, int count) : Card(original_name, name, count) { }

void CharacterCard::setLife(unsigned int life) {
    life_ = life;
}

unsigned int CharacterCard::getLife() const {
    return life_;
}
