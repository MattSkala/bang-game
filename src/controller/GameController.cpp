#include <iostream>
#include "GameController.h"

using namespace std;

GameController::GameController(Game & game, GameClient & client) : game_(game), client_(client) {
    listener_ = std::bind(&GameController::onStreamEvent, this, std::placeholders::_1);
}

void GameController::updatePlayersInfo() {
    vector<vector<string>> players = client_.getPlayersInfo();
    for (vector<string> player : players) {
        game_.updatePlayer(player[0], stoi(player[1]), player[2], player[3], stoi(player[4]));
    }
}

void GameController::actionInit() {
    updatePlayersInfo();

    vector<string> names = client_.getCards();
    vector<shared_ptr<Card>> cards = game_.getCardsByNames(names);
    game_.getMe()->setCards(cards);

    client_.addListener(listener_);
    renderBoard();

    while (true) {
        getchar();
    }
}

void GameController::renderBoard() {
    clearScreen();
    printLogo();

    cout << "Hráči:" << endl;
    vector<Player *> players = game_.getPlayers();
    for (unsigned int i = 0; i < players.size(); i++) {
        Player * player = players[i];
        char c = 'A' + i;
        cout << "[" << c << "] ";

        if (player == game_.getPlayerOnTurn()) {
            cout << "<";
        }
        if (player == game_.getMe()) {
            cout << "{";
        }
        cout << player->getName();
        if (player == game_.getMe()) {
            cout << "}";
        }
        if (player == game_.getPlayerOnTurn()) {
            cout << ">";
        }

        cout << " ~ ";
        if (player->getRole() != NULL) {
            cout << player->getRole()->getName() + " ~ ";
        }
        cout << player->getCharacter()->getName();
        cout << "  \u2665 " << player->getLife();
        cout << endl;
    }

    cout << endl;

    cout << "Karty na ruce:" << endl;
    vector<shared_ptr<Card>> cards = game_.getMe()->getCards();
    for (unsigned int i = 0; i < cards.size(); i++) {
        cout << "[" << i << "] " << cards[i]->getName() << " (" << cards[i]->getOriginalName() << ")" << endl;
    }

    cout << endl;

    if (game_.getPlayerOnTurn() == game_.getMe()) {
        cout << "Jsi na řadě!" << endl << endl;
    }
}

void GameController::actionRefresh() {
    renderBoard();
}

bool GameController::onStreamEvent(vector<string> event) {
    cout << "stream event:" << event[0] << endl;
    if (event[0] == "NEXT_ROUND" && event[1] == game_.getMe()->getName()) {
        updatePlayersInfo();
        renderBoard();
    }
    return true;
}
