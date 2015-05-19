#include <iostream>
#include <unistd.h>
#include <thread>
#include "BootstrapController.h"
#include "../net/GameServer.h"


using namespace std;


BootstrapController::BootstrapController(Game & game, GameClient & client, pid_t & server_pid) : game_(game), client_(client), server_pid_(server_pid) {
    listener_ = std::bind(&BootstrapController::onStreamEvent, this, std::placeholders::_1);
}

void BootstrapController::renderWelcome() {
    clearScreen();
    printLogo();
    cout << "Přejete si založit novou hru, nebo se připojit k již existující?" << endl << endl;
    cout << "o [Z]aložit hru" << endl;
    cout << "o [P]řipojit se k existující hře" << endl;

    while (true) {
        int c = getchar();
        if (toupper(c) == 'Z') {
            actionHostGame();
            break;
        } else if (toupper(c) == 'P') {
            actionJoinGame();
            break;
        }
    }
}

string BootstrapController::renderNameInput() {
    clearScreen();
    printLogo();
    cout << "Vaše jméno: " << endl;
    string name;
    cin >> name;

    return name;
}

void BootstrapController::renderPlayersList() {
    clearScreen();
    printLogo();
    cout << "Připojení hráči:" << endl;
    vector<Player *> & players = game_.getPlayers();
    Player * me = game_.getMe();
    for (unsigned int i = 0; i < players.size(); i++) {
        if (players[i]->getName() == me->getName()) {
            cout << "o " << players[i]->getName() << endl;
        } else {
            cout << "– " << players[i]->getName() << endl;
        }
    }
    cout << endl;

    if (server_pid_) {
        cout << "Přidat [A]I" << endl;
        cout << "[S]tart" << endl;
    } else {
        cout << "Čekání na spuštění hry..." << endl;
    }
}


bool BootstrapController::onStreamEvent(vector<string> event) {
    if (event[0] == "JOIN") {
        if (game_.getMe()->getName() != event[1]) {
            Player *player = new Player();
            player->setName(event[1]);
            game_.addPlayer(player);
            renderPlayersList();
        }
        return true;
    } else if (event[0] == "LEAVE") {
        cout << "LEAVE" << endl;
        vector<Player*> & players = game_.getPlayers();
        for (unsigned int i = 0; i < players.size(); i++) {
            if (players[i]->getName().compare(event[1]) == 0) {
                players.erase(players.begin() + i);
                break;
            }
        }
        renderPlayersList();
        return true;
    } else if (event[0] == "START") {
        started_ = true;
        return true;
    }

    return false;
}

void BootstrapController::renderServerInput() {
    clearScreen();
    printLogo();
    cout << "Zadejte IP adresu serveru:" << endl;
    string ip;
    cin >> ip;
    try {
        client_.connect(ip, GameServer::PORT);
    } catch (const char *err) {
        cout << err << endl;
        system("read");
        renderServerInput();
    }
}

void BootstrapController::actionHostGame() {
    // Start game server in a child process
    server_pid_ = fork();
    if (server_pid_ == 0) {
        GameServer server = GameServer(game_);
        server.start();
        exit(0);
    }

    if (game_.getMe() == NULL) {
        string name = renderNameInput();
        Player * player = new Player();
        player->setName(name);
        game_.setMe(player);
    }

    // wait for server to start
    usleep(100 * 1000);

    // connect to server
    client_.connect("127.0.0.1", GameServer::PORT);
    client_.join(game_.getMe()->getName());
    client_.addListener(listener_);

    renderPlayersList();

    while (true) {
        int c = getchar();
        if (toupper(c) == 'A') {
            actionAddBot();
        } else if (toupper(c) == 'S') {
            actionStartGame();
            break;
        }
    }
}

void BootstrapController::actionAddBot() {
    client_.addBot();
}

void BootstrapController::actionJoinGame() {
    string my_name = renderNameInput();
    renderServerInput();
    client_.join(my_name);
    client_.addListener(listener_);

    vector<string> names = client_.getPlayers();
    for (string name : names) {
        if (my_name == name) {
            Player * me = new Player();
            me->setName(my_name);
            game_.setMe(me);
            continue;
        }
        Player *player = new Player();
        player->setName(name);
        game_.addPlayer(player);
    }

    renderPlayersList();

    while (!started_) {
        sleep(1);
    }
}

void BootstrapController::actionStartGame() {
    if (game_.getPlayers().size() < GameServer::MIN_PLAYERS || game_.getPlayers().size() > GameServer::MAX_PLAYERS) {
        cout << "Pro spuštění hry je potřeba " << GameServer::MIN_PLAYERS << " – " << GameServer::MAX_PLAYERS << " hráčů." << endl;
        while (game_.getPlayers().size() < GameServer::MIN_PLAYERS || game_.getPlayers().size() > GameServer::MAX_PLAYERS) {
            getchar();
        }
    }

    client_.removeListener(listener_);
    client_.startGame();
}
