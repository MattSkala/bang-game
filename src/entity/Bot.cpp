#include <unistd.h>
#include <set>
#include <list>
#include <iostream>
#include "Bot.h"
#include "MissCard.h"
#include "BeerCard.h"
#include "../net/GameServer.h"
#include "../Game.h"

using namespace std;

Bot::Bot() { }

void Bot::play(GameServer * server, Game * game) {
    sleep(2);

    int position, target, target_card;
    while (getBestMove(game, position, target, target_card)) {
        server->playCard(this, position, target, target_card);
        while (game->getPendingPlayers().size() > 0) {
            sleep(1);
        }
        sleep(2);
    }

    discardExcessCards(game);
}

void Bot::reply(GameServer * server, Game * game) {
    sleep(2);

    auto & cards = getCards();

    int miss = -1;
    int beer = -1;
    for (int i = 0; i < (int) cards.size(); i++) {
        if (dynamic_pointer_cast<MissCard>(cards[i])) {
            miss = i;
        }
        shared_ptr<BeerCard> beer_card = dynamic_pointer_cast<BeerCard>(cards[i]);
        if (beer_card && !beer_card->isTargetable()) {
            beer = i;
        }
    }

    if (miss >= 0 && server->playCard(this, miss, -1, -1) == to_string(Game::SUCCESS)) {
        return;
    }

    if (beer >= 0 && server->playCard(this, beer, -1, -1) == to_string(Game::SUCCESS)) {
        return;
    }

    server->proceed(this);
}

void Bot::discardExcessCards(Game * game) {
    while (getCards().size() > getMaxLife()) {
        game->discardCard(this, (int) getMaxLife());
        sleep(1);
    }
}

bool Bot::getBestMove(Game * game, int & position, int & target, int & target_card) {
    set<vector<int>, bool(*)(vector<int>, vector<int>)> moves([](vector<int> l, vector<int> r) {
        // Maximize own utility and minimize opponent's utility
        return l[0] > r[0] || l[1] < r[1];
    });

    auto & cards = getCards();
    auto & players = game->getPlayers();

    for (int i = 0; i < (int) cards.size(); i++) {
        for (int j = 0; j < (int) players.size(); j++) {
            for (int k = -1; k < (int) players[j]->getPermanentCards().size(); k++) {
                Game state = game->clone();
                Player * me = state.getPlayer(getName());

                if (state.playCard(me, i, j, k) == Game::SUCCESS) {
                    for (Player * player : state.getPendingPlayers()) {
                        state.proceed(player);
                    }

                    int opponent_utility = 0;
                    vector<Player *> opponents = state.getOpponents(me);
                    for (Player * opponent : opponents) {
                        opponent_utility += state.computeUtility(opponent);
                    }

                    vector<int> move(5);
                    move[0] = state.computeUtility(me);
                    move[1] = opponent_utility;
                    move[2] = i;
                    move[3] = cards[i]->isTargetable() ? j : -1;
                    move[4] = k;
                    moves.insert(move);
                }

                if (!cards[i]->isCardTargetable()) {
                    break;
                }
            }

            if (!cards[i]->isTargetable()) {
                break;
            }
        }
    }

    if (moves.size() == 0) {
        return false;
    }

    // Return best move
    vector<int> best_move = *moves.begin();
    position = best_move[2];
    target = best_move[3];
    target_card = best_move[4];

    return true;
}
