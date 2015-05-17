#include "Game.h"


Game::Game() : me_(NULL) { }

Game::~Game() {
    for (Player *player : players_) {
        delete player;
    }
}

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
    vector<shared_ptr<RoleCard>> roles;
    vector<shared_ptr<CharacterCard>> characters;
    vector<shared_ptr<Card>> pack;
    for (Card* card : cards) {
        if (RoleCard* role = dynamic_cast<RoleCard*>(card)) {
            roles.push_back(shared_ptr<RoleCard>(role));
        } else if (CharacterCard* character = dynamic_cast<CharacterCard*>(card)) {
            characters.push_back(shared_ptr<CharacterCard>(character));
        } else {
            shared_ptr<Card> card_ptr = shared_ptr<Card>(card);
            for (int i = 0; i < card->getCount(); i++) {
                pack.push_back(card_ptr);
            }
        }
    }
    roles_ = roles;
    characters_ = characters;
    pack_ = pack;
}
