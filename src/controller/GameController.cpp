#include <iostream>
#include "GameController.h"

using namespace std;

GameController::GameController(Game & game, GameClient & client) : game_(game), client_(client) { }

void GameController::actionInit() {
    cout << "init" << endl;
    vector<string> names = client_.getCards();
    cout << "got cards:" << names.size() << endl;
    vector<shared_ptr<Card>> cards = game_.getCardsByNames(names);
    cout << "got card by names:" << cards.size() << endl;
    game_.getMe()->setCards(cards);
    cout << "set cards" << endl;

    renderBoard();
}

void GameController::renderBoard() {
    clearScreen();
    printLogo();

    cout << "Hráči:" << endl;
    for (Player *player : game_.getPlayers()) {
        if (player->getName() == game_.getMe()->getName()) {
            cout << "o " << player->getName() << " \t\u2665 " << player->getLife() << endl;
        } else {
            cout << "- " << player->getName() << " \t\u2665 " << player->getLife() << endl;
        }
    }

    cout << endl;

    cout << "Karty na ruce:" << endl;
    for (shared_ptr<Card> card : game_.getMe()->getCards()) {
        cout << "– " << card->getName() << " (" << card->getOriginalName() << ")" << endl;
    }

    system("read");
}

void GameController::actionRefresh() {
    renderBoard();
}
