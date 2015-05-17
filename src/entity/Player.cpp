#include "Player.h"

Player::Player() {
}

void Player::setName(std::string name) {
    name_ = name;
}

std::string Player::getName() const {
    return name_;
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
