#include <algorithm>
#include <stdlib.h>
#include <iostream>
#include "Game.h"
#include "net/GameServer.h"

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

int Game::getPendingPlayersCount() const {
    int pending = 0;
    for (Player *item : players_) {
        if (item->isPending()) {
            pending++;
        }
    }
    return pending;
}

void Game::setPack(vector<Card *> cards) {
    vector<shared_ptr<RoleCard>> roles;
    vector<shared_ptr<CharacterCard>> characters;
    vector<shared_ptr<PlayableCard>> pack;
    for (Card* card : cards) {
        if (RoleCard* role = dynamic_cast<RoleCard*>(card)) {
            roles.push_back(shared_ptr<RoleCard>(role));
        } else if (CharacterCard* character = dynamic_cast<CharacterCard*>(card)) {
            characters.push_back(shared_ptr<CharacterCard>(character));
        } else if (PlayableCard* playable = dynamic_cast<PlayableCard*>(card)) {
            shared_ptr<PlayableCard> card_ptr = shared_ptr<PlayableCard>(playable);
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
    RoleCard * deputy = NULL;
    RoleCard * outlaw = NULL;
    RoleCard * renegate = NULL;
    for (shared_ptr<RoleCard> role : roles_) {
        if (role->getOriginalName() == RoleCard::SHERIFF) {
            sheriff = role.get();
        } else if (role->getOriginalName() == RoleCard::DEPUTY) {
            deputy = role.get();
        } else if (role->getOriginalName() == RoleCard::OUTLAW) {
            outlaw = role.get();
        } else if (role->getOriginalName() == RoleCard::RENEGATE) {
            renegate = role.get();
        }
    }

    srand((unsigned int) time(0));

    // 1 sheriff
    int sheriff_position = rand() % (int) players_.size();
    players_[sheriff_position]->setRole(sheriff);

    // 1 renegate
    if (players_.size() >= 4) {
        int renegate_position;
        do {
            renegate_position = rand() % (int) players_.size();
        } while (renegate_position == sheriff_position);
        players_[renegate_position]->setRole(renegate);
    }

    // 1 or 2 deputies
    int deputies = 0;
    if (players_.size() >= 5) deputies++;
    if (players_.size() >= 7) deputies++;
    while (deputies > 0) {
        int deputy_position = rand() % (int) players_.size();
        if (players_[deputy_position]->getRole() == NULL) {
            players_[deputy_position]->setRole(deputy);
            deputies--;
        }
    }

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
        for (unsigned int i = 0; i < player->getMaxLife(); i++) {
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
    setLifePoints();
    assignCards();
    player_on_turn_ = players_[0];
}

shared_ptr<PlayableCard> Game::getCard(string name) {
    for (shared_ptr<PlayableCard> card : cards_) {
        if (card->getOriginalName() == name) {
            return card;
        }
    }
    return NULL;
}

vector<shared_ptr<PlayableCard>> Game::getCardsByNames(vector<string> names) const {
    vector<shared_ptr<PlayableCard>> cards;
    for (string name : names) {
        for (shared_ptr<PlayableCard> card : cards_) {
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

vector<Player *> Game::getPlayersByRole(string role) {
    vector<Player *> players;
    for (Player * player : players_) {
        if (player->getRole()->getOriginalName() == role) {
            players.push_back(player);
        }
    }
    return players;
}

vector<string> Game::getWinners() {
    vector<string> winners;

    Player * sheriff = getPlayersByRole(RoleCard::SHERIFF)[0];
    Player * renegate = getPlayersByRole(RoleCard::RENEGATE)[0];
    vector<Player *> deputies = getPlayersByRole(RoleCard::DEPUTY);
    vector<Player *> outlaws = getPlayersByRole(RoleCard::OUTLAW);

    bool deputies_dead = true;
    bool outlaws_dead = true;

    for (Player * player : deputies) {
        if (player->isAlive()) {
            deputies_dead = false;
            break;
        }
    }

    for (Player * player : outlaws) {
        if (player->isAlive()) {
            outlaws_dead = false;
            break;
        }
    }

    if (sheriff->isAlive()) {
        if (outlaws_dead && !renegate->isAlive()) {
            winners.push_back(RoleCard::SHERIFF);
            if (deputies.size() > 0) {
                winners.push_back(RoleCard::DEPUTY);
            }
        }
    } else {
        if (deputies_dead && outlaws_dead) {
            winners.push_back(RoleCard::RENEGATE);
        } else {
            winners.push_back(RoleCard::OUTLAW);
        }
    }

    return winners;
}

int Game::getPlayerPosition(Player *player) const {
    int position = 0;
    for (unsigned int i = 0; i < players_.size(); i++) {
        if (players_[i] == player) {
            position = i;
            break;
        }
    }
    return position;
}

int Game::getDistance(Player *player, int target) const {
    int position = getPlayerPosition(player);
    int d1 = abs(position - target);
    int d2;
    if (position > target) {
        d2 = (int) players_.size() - position + target;
    } else {
        d2 = (int) players_.size() + position - target;
    }
    int d = d1 < d2 ? d1 : d2;

    // Appaloosa
    int from = player->getDistanceFrom();
    d += from;

    // Mustang
    int to = players_[target]->getDistanceTo();
    d += to;

    if (d < 1) {
        d = 1;
    }

    return d;
}

Player * Game::getPlayer(string name) {
    for (Player *player : players_) {
        if (player->getName() == name) {
            return player;
        }
    }
    return NULL;
}

bool Game::updatePlayer(string name, unsigned int life, string character, string role, bool on_turn, bool pending, unsigned int cards) {
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

    player->setPending(pending);

    player->setCardsCount(cards);

    return true;
}

bool Game::updatePermanentCards(string name, vector<string> card_names) {
    Player * player = getPlayer(name);
    if (player == NULL) {
        return false;
    }

    vector<shared_ptr<PermanentCard>> cards;
    for (string card_name : card_names) {
        shared_ptr<PermanentCard> card = dynamic_pointer_cast<PermanentCard>(getCard(card_name));
        cards.push_back(card);
    }

    player->setPermanentCards(cards);
    return true;
}

void Game::drawCard(Player *player) {
    player->addCard(pack_.front());
    pack_.pop_front();
}

bool Game::discardCard(Player *player, int position) {
    vector<shared_ptr<PlayableCard>> & cards = player->getCards();
    if ((int) cards.size() > position) {
        pack_.push_back(cards[position]);
        cards.erase(cards.begin() + position);
        return true;
    }
    return false;
}

void Game::discardCard(shared_ptr<PlayableCard> card) {
    pack_.push_back(card);
}

int Game::playCard(Player *player, int position, int target, int target_card) {
    shared_ptr<PlayableCard> ptr = player->getCards()[position];
    PlayableCard * card = ptr.get();
    return card->play(this, player, position, target, target_card);
}

void Game::finishRound() {
    for (Player * player : players_) {
        player->setPlayedBang(false);
    }

    if (players_.back() == player_on_turn_) {
        player_on_turn_ = players_.front();
    } else {
        for (unsigned int i = 0; i < players_.size(); i++) {
            if (players_[i] == player_on_turn_) {
                player_on_turn_ = players_[i + 1];
                break;
            }
        }
    }
}

void Game::setPendingCard(PlayableCard *card) {
    pending_card_ = card;
}

PlayableCard * Game::getPendingCard() {
    return pending_card_;
}

void Game::proceed(Player *player) {
    pending_card_->proceed(this, player);

    player->setPending(false);

    if (getPendingPlayersCount() == 0) {
        pending_card_ = NULL;
    }
}

string Game::getErrorMessage(int code) {
    switch (code) {
        case ERROR_INVALID_CARD:
            return "Neplatný kód zahrané karty.";
        case ERROR_INVALID_TARGET:
            return "Neplatný kód cílového hráče.";
        case ERROR_INVALID_TARGET_CARD:
            return "Neplatný kód cílové karty.";
        case ERROR_OUT_OF_RANGE:
            return "Cílový hráč je moc daleko.";
        case ERROR_BANG_LIMIT:
            return "Toto kolo jsi již kartu Bang! zahrál.";
        case ERROR_TARGET_NO_CARDS:
            return "Cílový hráč nemá v ruce žádné karty.";
        case ERROR_CARD_ALREADY_LAID:
            return "Tuto kartu máš již vyloženou.";
        case ERROR_INVALID_MISS:
            return "Tuto kartu lze zahrát pouze jako reakci na útok.";
        case ERROR_INVALID_REACTION:
            return "Nyní lze zahrát pouze kartu reagující na útok.";
        case ERROR_UNKNOWN_CARD:
            return "Neplatná definice karty.";
        default:
            return "Nastala neznámá chyba.";
    }
}
