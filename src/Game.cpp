#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include "Game.h"

using namespace std;

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
    cards_ = pack;
}

void Game::assignRoles() {
    RoleCard * sheriff = NULL;
    //RoleCard * deputy = NULL;
    RoleCard * outlaw = NULL;
    RoleCard * renegate = NULL;
    for (shared_ptr<RoleCard> role : roles_) {
        if (role->getOriginalName() == RoleCard::SHERIFF) {
            sheriff = role.get();
        } else if (role->getOriginalName() == RoleCard::DEPUTY) {
            //deputy = role.get();
        } else if (role->getOriginalName() == RoleCard::OUTLAW) {
            outlaw = role.get();
        } else if (role->getOriginalName() == RoleCard::RENEGATE) {
            renegate = role.get();
        }
    }

    srand((unsigned int) time(0));

    // 1 sheriff
    unsigned long sheriff_position = rand() % players_.size();
    players_[sheriff_position]->setRole(sheriff);

    // 1 renegate
    if (players_.size() >= 4) {
        unsigned long renegate_position;
        do {
            renegate_position = rand() % players_.size();
        } while (renegate_position == sheriff_position);
        players_[renegate_position]->setRole(renegate);
    }

    // TODO: 1 or 2 deputies

    // 2 or 3 outlaws
    for (Player * player : players_) {
        if (player->getRole() == NULL) {
            player->setRole(outlaw);
        }
    }
}

void Game::assignCharacters() {
    // shuffle characters cards;
    srand((unsigned int) time(0));
    random_shuffle(characters_.begin(), characters_.end());

    for (unsigned int i = 0; i < players_.size(); i++) {
        shared_ptr<CharacterCard> ptr = characters_[i];
        CharacterCard * card = ptr.get();
        players_[i]->setCharacter(card);
    }
}

void Game::assignCards() {
    // shuffle cards
    srand((unsigned int) time(0));
    random_shuffle(cards_.begin(), cards_.end());

    // copy cards to pack
    copy(cards_.begin(), cards_.end(), back_inserter(pack_));

    // assign initial cards to players hands
    for (Player * player : players_) {
        for (int i = 0; i < player->getCharacter()->getLife(); i++) {
            player->addCard(pack_.front());
            pack_.pop_front();
        }
    }
}

void Game::setLifePoints() {
    for (Player * player : players_) {
        player->setLife(player->getMaxLife());
    }
}

void Game::initGame() {
    assignRoles();
    assignCharacters();
    assignCards();
    setLifePoints();
    player_on_turn_ = players_[0];
}

shared_ptr<Card> Game::getCard(string name) {
    for (shared_ptr<Card> card : cards_) {
        if (card->getOriginalName() == name) {
            return card;
        }
    }
    return NULL;
}

vector<shared_ptr<Card>> Game::getCardsByNames(vector<string> names) const {
    vector<shared_ptr<Card>> cards;
    for (string name : names) {
        for (shared_ptr<Card> card : cards_) {
            if (card->getOriginalName() == name) {
                cards.push_back(card);
                break;
            }
        }
    }
    return cards;
}

Player * Game::getPlayerOnTurn() {
    return player_on_turn_;
}

Player * Game::getPlayer(string name) {
    for (Player * player : players_) {
        if (player->getName() == name) {
            return player;
        }
    }
    return NULL;
}

bool Game::updatePlayer(string name, int life, string character, string role, bool on_turn) {
    Player * player = getPlayer(name);
    if (player == NULL) {
        return false;
    }

    player->setLife(life);

    if (player->getCharacter() == NULL) {
        for (shared_ptr<CharacterCard> card : characters_) {
            if (card->getOriginalName() == character) {
                player->setCharacter(card.get());
                break;
            }
        }
    }

    if (player->getRole() == NULL && role != "?") {
        for (shared_ptr<RoleCard> card : roles_) {
            if (card->getOriginalName() == role) {
                player->setRole(card.get());
                break;
            }
        }
    }

    if (on_turn) {
        player_on_turn_ = player;
    }

    return true;
}

bool Game::drawCards(Player *player) {
    if (player != getPlayerOnTurn()) {
        return false;
    }

    if ((int) player->getCards().size() != player->getMaxLife()) {
        return false;
    }

    drawCard(player);
    drawCard(player);
    return true;
}

void Game::drawCard(Player *player) {
    player->addCard(pack_.front());
    pack_.pop_front();
}
