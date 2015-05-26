#include <iostream>
#include <unistd.h>
#include <thread>
#include "BootstrapController.h"
#include "../Exception.h"
#include "../net/GameServer.h"


using namespace std;

const string BootstrapController::LOCALHOST = "127.0.0.1";

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
        string s;
        getline(cin, s);
        char c;
        if (scanChar(s, c)) {
            if (toupper(c) == 'Z') {
                actionHostGame();
                break;
            } else if (toupper(c) == 'P') {
                actionJoinGame();
                break;
            }
        }
    }
}

string BootstrapController::renderNameInput() {
    clearScreen();
    printLogo();
    cout << "Vaše jméno: " << endl;
    string name;
    getline(cin, name);

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

string BootstrapController::renderServerInput() {
    clearScreen();
    printLogo();
    cout << "Zadejte IP adresu serveru:" << endl;
    string server_ip;
    getline(cin, server_ip);
    return server_ip;
}

void BootstrapController::actionHostGame() {
    // Start game server in a child process
    server_pid_ = fork();
    if (server_pid_ == 0) {
        try {
            GameServer server = GameServer(game_);
            server.start();
        } catch (Exception e) {
            cerr << e.getName() << ": " << e.getMessage() << endl;
        }
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
    client_.connect(LOCALHOST, GameServer::PORT);
    client_.join(game_.getMe()->getName());
    client_.addListener(listener_);

    renderPlayersList();

    while (true) {
        string s;
        getline(cin, s);
        char c;
        if (scanChar(s, c)) {
            if (toupper(c) == 'A') {
                actionAddBot();
            } else if (toupper(c) == 'S') {
                if (actionStartGame()) {
                    break;
                }
            }
        }
    }
}

void BootstrapController::actionAddBot() {
    if (!client_.addBot()) {
        renderPlayersList();
    }
}

void BootstrapController::actionJoinGame() {
    string my_name = renderNameInput();

    string server_ip;
    while (server_ip.size() == 0) {
        server_ip = renderServerInput();
        try {
            client_.connect(server_ip, GameServer::PORT);
            client_.join(my_name);
        } catch (Exception err) {
            cerr << err.getMessage() << endl;
            server_ip.clear();
            string tmp;
            getline(cin, tmp);
        }
    }

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

bool BootstrapController::actionStartGame() {
    if (game_.getPlayers().size() < GameServer::MIN_PLAYERS || game_.getPlayers().size() > GameServer::MAX_PLAYERS) {
        cout << "Pro spuštění hry je potřeba " << GameServer::MIN_PLAYERS << " – " << GameServer::MAX_PLAYERS << " hráčů." << endl;
        string s;
        getline(cin, s);
        renderPlayersList();
        return false;
    }

    client_.removeListener(listener_);
    client_.startGame();

    return true;
}
