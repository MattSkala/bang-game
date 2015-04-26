#include <iostream>
#include "BootstrapController.h"

using namespace std;


BootstrapController::BootstrapController(Game & game) : game_(game) { }


void BootstrapController::renderWelcome() {
    clearScreen();
    printLogo();
    cout << "Přejete si založit novou hru, nebo se připojit k již existující?" << endl << endl;
    cout << "o [Z]aložit hru" << endl;
    cout << "o [P]řipojit se k existující hře" << endl;

    while (true) {
        int c = getchar();
        if (c == 'z' || c == 'Z') {
            actionHostGame();
            break;
        } else if (c == 'p' || c == 'P') {
            actionJoinGame();
            break;
        }
    }
}


void BootstrapController::renderNameInput() {
    clearScreen();
    printLogo();
    cout << "Vaše jméno: " << endl;
    string name;
    cin >> name;

    Player * player = new Player();
    player->setName(name);
    game_.setMe(player);
}


void BootstrapController::renderPlayersList() {
    clearScreen();
    printLogo();
    cout << "Připojení hráči:" << endl;
    vector<Player *> & players = game_.getPlayers();
    for (int i = 0; i < players.size(); i++) {
        cout << "o " << players[i]->getName() << endl;
    }
    cout << endl;
    cout << "Přidat [A]I" << endl;
    cout << "[S]tart" << endl;

    while (true) {
        int c = getchar();
        if (c == 'a' || c == 'A') {
            actionAddBot();
            break;
        } else if (c == 's' || c == 'S') {
            actionStartGame();
            break;
        }
    }
}


void BootstrapController::actionHostGame() {
    if (game_.getMe() == NULL) {
        renderNameInput();
    }

    // TODO: start a game server
    renderPlayersList();
}


void BootstrapController::actionAddBot() {
    Player * player = new Player();
    player->setName("Bot");
    game_.addPlayer(player);
    renderPlayersList();
}


void BootstrapController::actionJoinGame() {
    // TODO: discover game servers in LAN
    renderWelcome();
}


void BootstrapController::actionStartGame() {
    // TODO: transfer control to GameController
    renderPlayersList();
}
