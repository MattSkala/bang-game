#include "Player.h"

Player::Player() {
}

void Player::setName(std::string name) {
    name_ = name;
}

std::string Player::getName() const {
    return name_;
}

void Player::setRole(RoleCard *card) {
    role_ = card;
}

RoleCard* Player::getRole() const {
    return role_;
}

void Player::setCharacter(CharacterCard *card) {
    character_ = card;
}

CharacterCard* Player::getCharacter() const {
    return character_;
}

void Player::addCard(shared_ptr<Card> card) {
    cards_.push_back(card);
}

void Player::setCards(vector<shared_ptr<Card>> cards) {
    cards_ = cards;
}

vector<shared_ptr<Card>>& Player::getCards() {
    return cards_;
}

int Player::getLife() const {
    return life_;
}

void Player::decreaseLife() {
    if (life_ > 0) {
        life_--;
    }
}

bool Player::increaseLife() {
    if (max_life_ == life_) {
        return false;
    }
    life_++;
    return true;
}
