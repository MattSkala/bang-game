#include "Game.h"


Game::Game() : me_(NULL) { }


void Game::setMe(Player *player) {
    me_ = player;
    players_.push_back(me_);
}


Player * Game::getMe() {
    return me_;
}


void Game::addPlayer(Player *player) {
    players_.push_back(player);
}


vector<Player *>& Game::getPlayers() {
    return players_;
}


void Game::setPack(vector<Card *> cards) {
    pack_ = cards;
}
