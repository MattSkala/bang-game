#include <iostream>
#include "Player.h"
#include "GunCard.h"

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

void Player::layCard(int position) {
    shared_ptr<PermanentCard> card = dynamic_pointer_cast<PermanentCard>(cards_[position]);
    cards_.erase(cards_.begin() + position);
    permanents_.push_back(card);
}

vector<shared_ptr<PlayableCard>>& Player::getCards() {
    return cards_;
}

void Player::setPermanentCards(vector<shared_ptr<PermanentCard>> cards) {
    permanents_ = cards;
}

vector<shared_ptr<PermanentCard>>& Player::getPermanentCards() {
    return permanents_;
}

int Player::getGunRange() const {
    for (auto & card : permanents_) {
        shared_ptr<GunCard> gun = dynamic_pointer_cast<GunCard>(card);
        if (gun) {
            return gun->getDistance();
        }
    }
    return 1;
}

bool Player::hasUnlimitedBang() const {
    for (auto & card : permanents_) {
        shared_ptr<GunCard> gun = dynamic_pointer_cast<GunCard>(card);
        if (gun) {
            return gun->hasUnlimitedBang();
        }
    }
    return false;
}

int Player::getDistanceFrom() const {
    int distance = 0;
    for (auto card : permanents_) {
        int tweak = card->getDistanceTweak();
        if (tweak < 0) {
            distance += tweak;
        }
    }
    return distance;
}

int Player::getDistanceTo() const {
    int distance = 0;
    for (auto card : permanents_) {
        int tweak = card->getDistanceTweak();
        if (tweak > 0) {
            distance += tweak;
        }
    }
    return distance;
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

void Player::setPlayedBang(bool played) {
    played_bang_ = played;
}

bool Player::hasPlayedBang() const {
    return played_bang_;
}
