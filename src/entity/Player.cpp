#include <iostream>
#include "Player.h"

using namespace std;

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

void Player::addCard(shared_ptr<PlayableCard> card) {
    cards_.push_back(card);
}

void Player::setCards(vector<shared_ptr<PlayableCard>> cards) {
    cards_ = cards;
}

vector<shared_ptr<PlayableCard>>& Player::getCards() {
    return cards_;
}

unsigned int Player::getLife() const {
    return life_;
}

bool Player::isAlive() const {
    return life_ > 0;
}

void Player::decreaseLife() {
    if (life_ > 0) {
        life_--;
    }
}

bool Player::increaseLife() {
    if (getMaxLife() == life_) {
        return false;
    }
    life_++;
    return true;
}

void Player::setLife(unsigned int life) {
    life_ = life;
}

unsigned int Player::getMaxLife() const {
    unsigned int life = character_->getLife();
    if (role_->getOriginalName() == RoleCard::SHERIFF) {
        life++;
    }
    return life;
}

void Player::setPending(bool pending) {
    pending_ = pending;
}

bool Player::isPending() const {
    return pending_;
}
