#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <sstream>
#include <string.h>
#include "../Exception.h"
#include "../util/utils.h"
#include "../entity/Bot.h"
#include "GameServer.h"

#define BUFFER_SIZE 1024

const string GameServer::SUCCESS = "OK";
const string GameServer::ERROR = "ERROR";
const string GameServer::ERROR_JOIN_NAME = "JOIN_NAME";
const string GameServer::ERROR_JOIN_PLAYING = "JOIN_PLAYING";
const string GameServer::ERROR_BOT_LIMIT = "BOT_LIMIT";

const string GameServer::ERROR_ALREADY_RUNNING = "ALREADY_RUNNING";
const string GameServer::ERROR_NOT_RUNNING = "NOT_RUNNING";
const string GameServer::ERROR_SETSOCKOPT = "SETSOCKOPT";
const string GameServer::ERROR_BIND = "BIND";
const string GameServer::ERROR_LISTEN = "LISTEN";
const string GameServer::ERROR_ACCEPT = "ACCEPT";
const string GameServer::ERROR_RECV = "RECV";
const string GameServer::ERROR_SEND = "SEND";
const string GameServer::ERROR_SUBSCRIBE = "SUBSCRIBE";
const string GameServer::ERROR_GETADDRINFO = "GETADDRINFO";
const string GameServer::ERROR_SOCKET = "SOCKET";


GameServer::GameServer(Game &game) : game_(game) { }

GameServer::~GameServer() {
    stop();
}

void GameServer::start() {
    if (running_) {
        throw Exception(ERROR_ALREADY_RUNNING, "Server is already running.");
    }

    struct addrinfo * servinfo = getServerInfo();
    socket_ = getSocket(servinfo);

    // Allow reusing address to prevent "Address already in use" error
    int yes = 1;
    if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
        throw Exception(ERROR_SETSOCKOPT, strerror(errno));
    }

    // Bind a socket to a port on our local address
    if (::bind(socket_, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {
        close(socket_);
        throw Exception(ERROR_BIND, strerror(errno));
    }

    // Free address info – not needed anymore
    freeaddrinfo(servinfo);

    if (listen(socket_, GameServer::MAX_PLAYERS) == -1) {
        throw Exception(ERROR_LISTEN, strerror(errno));
    }

    running_ = true;

    while (running_) {
        waitForConnection();
    }
}

/**
 * Waits for new connections and incoming requests.
 */
void GameServer::waitForConnection() {
    fd_set readfds;
    FD_ZERO(&readfds);

    // max socket value
    int max = socket_;

    // add bound server socket to read fd set
    FD_SET(socket_, &readfds);

    // add all connected clients to read fd set
    for (vector<int>::size_type i = 0; i < connections_.size(); i++) {
        FD_SET(connections_[i], &readfds);
        if (connections_[i] > max) {
            max = connections_[i];
        }
    }

    // synchronous I/O multiplexing
    select(max + 1, &readfds, NULL, NULL, NULL);

    // handle new connection
    if (FD_ISSET(socket_, &readfds)) {
        acceptNewConnection();
    }

    for (vector<int>::iterator it = connections_.begin(); it < connections_.end(); it++) {
        // incoming request from client
        int i = it - connections_.begin();
        if (FD_ISSET(connections_[i], &readfds)) {
            string req;

            bool result = false;
            try {
                result = receiveRequest(connections_[i], req);
            } catch (Exception ex) {
                cerr << ex.getMessage() << endl;
            }

            if (result) {
                string res = processRequest(req, i);
                sendResponse(connections_[i], res);
                // cout << req << " -> " << res << endl;
            } else {
                // client closed connection
                handleUserLeave(it);
            }
        }
    }
}

void GameServer::handleUserLeave(vector<int>::iterator it) {
    int socket = *it;

    // remove connection
    connections_.erase(it);

    // remove stream connection
    for (vector<int>::iterator it = stream_connections_.begin(); it < stream_connections_.end(); it++) {
        if (*it == socket) {
            stream_connections_.erase(it);
            break;
        }
    }

    // remove from players list
    Player *player = players_[socket];
    if (player != NULL) {
        string name = string(player->getName());
        vector<Player *> & players = game_.getPlayers();
        for (unsigned int i = 0; i < players.size(); i++) {
            if (player == players[i]) {
                players.erase(players.begin() + i);
                delete player;
                break;
            }
        }

        // remove from map
        players_.erase(socket);

        // send event
        sendEvent("LEAVE|" + name);
    }
}

/**
 * Accepts a new connection and adds a client socket to the connections pool.
 */
void GameServer::acceptNewConnection() {
    struct sockaddr_storage their_addr;
    socklen_t sin_size = sizeof their_addr;
    int client_socket = accept(socket_, (struct sockaddr *) &their_addr, &sin_size);

    if (client_socket == -1) {
        throw Exception(ERROR_ACCEPT, strerror(errno));
    }

    connections_.push_back(client_socket);
}

/**
 * Receives incoming request on provided client socket.
 */
bool GameServer::receiveRequest(int client_socket, string & req) {
    char buffer[BUFFER_SIZE];
    int received = 0;

    if ((received = recv(client_socket, buffer, BUFFER_SIZE, 0)) == -1) {
        throw Exception(ERROR_RECV, strerror(errno));
    }

    if (received == 0) {
        // client closed connection
        return false;
    }

    req = string(buffer, received);
    return true;
}

string GameServer::processRequest(string req, int connection) {
    string res;
    int socket = connections_[connection];
    Player * player = players_[socket];

    vector<string> args = explode(req, '|');

    if ("JOIN" == args[0]) {
        if (game_.getPlayerOnTurn() != NULL) {
            res = GameServer::ERROR_JOIN_PLAYING;
        } else if (game_.getPlayer(args[1]) != NULL) {
            res = GameServer::ERROR_JOIN_NAME;
        } else {
            // add user
            player = new Player();
            player->setName(args[1]);
            game_.addPlayer(player);
            players_[socket] = player;
            sendEvent("JOIN|" + args[1]);
            res = GameServer::SUCCESS;
        }
    } else if ("ADD_BOT" == args[0]) {
        if (game_.getBotsCount() >= 2) {
            res = GameServer::ERROR_BOT_LIMIT;
        } else {
            // add bot
            player = new Bot();
            int names_size = 6;
            string names[6] = {"Bill", "Frank", "Jim", "Annie", "Jeff", "Will"};
            do {
                player->setName(names[rand() % names_size]);
            } while (game_.getPlayer(player->getName()) != NULL);
            game_.addPlayer(player);
            sendEvent("JOIN|" + player->getName());
            res = GameServer::SUCCESS;
        }
    } else if ("GET_PLAYERS" == req) {
        res = "";
        vector<Player *> players = game_.getPlayers();
        for (unsigned int i = 0; i < players.size(); i++) {
            res += players[i]->getName();

            if (i < players.size() - 1) {
                res += ';';
            }
        }
    } else if ("GET_PLAYERS_INFO" == req) {
        res = "";
        vector<Player *> players = game_.getPlayers();
        for (unsigned int i = 0; i < players.size(); i++) {
            res += players[i]->getName();
            res += ',';
            res += to_string(players[i]->getLife());
            res += ',';
            res += players[i]->getCharacter()->getOriginalName();
            res += ',';
            if (players[i]->getRole()->getOriginalName() == RoleCard::SHERIFF
                || !players[i]->isAlive()
                || player->getName() == players[i]->getName()
                || game_.getWinners().size() > 0) {
                res += players[i]->getRole()->getOriginalName();
            } else {
                res += '?';
            }
            res += ',';
            res += to_string(game_.getPlayerOnTurn()->getName() == players[i]->getName());
            res += ',';
            res += to_string(players[i]->isPending());
            res += ',';
            res += to_string(players[i]->getCards().size());

            if (i < players.size() - 1) {
                res += ';';
            }
        }
    } else if ("SUBSCRIBE" == req) {
        stream_connections_.push_back(connections_[connection]);
        res = GameServer::SUCCESS;
    } else if ("START" == req) {
        game_.initGame();
        game_.drawCard(game_.getPlayerOnTurn());
        game_.drawCard(game_.getPlayerOnTurn());
        sendEvent("START");
        res = GameServer::SUCCESS;
    } else if ("GET_CARDS" == req) {
        vector<shared_ptr<PlayableCard>> cards = player->getCards();
        res = "";
        for (unsigned int i = 0; i < cards.size(); i++) {
            res += cards[i]->getOriginalName();
            if (i < cards.size() - 1) {
                res += ";";
            }
        }
    } else if ("GET_PERMANENT_CARDS" == req) {
        res = "";
        vector<Player *> players = game_.getPlayers();
        for (unsigned int i = 0; i < players.size(); i++) {
            res += players[i]->getName() + ":";
            vector<shared_ptr<PermanentCard>> cards = players[i]->getPermanentCards();
            for (unsigned int j = 0; j < cards.size(); j++) {
                res += cards[j]->getOriginalName();
                if (j < cards.size() - 1) {
                    res += ",";
                }
            }
            if (i < players.size() - 1) {
                res += ";";
            }
        }
    } else if ("FINISH_ROUND" == req) {
        res = finishRound();
    } else if ("DISCARD_CARD" == args[0]) {
        int position = stoi(args[1]);
        game_.discardCard(player, position);
        res = GameServer::SUCCESS;
    } else if ("PLAY_CARD" == args[0]) {
        int position = stoi(args[1]);
        int target = stoi(args[2]);
        int target_card = stoi(args[3]);
        res = playCard(player, position, target, target_card);
    } else if ("PROCEED" == args[0]) {
        res = proceed(player);
    } else {
        res = "INVALID_REQUEST";
    }

    return res;
}

void GameServer::startBot(Bot * bot) {
    thread bot_thread = thread(&GameServer::handleBot, this, bot);
    bot_thread.detach();
}

void GameServer::handleBot(Bot * bot) {
    if (bot->isPending()) {
        bot->reply(this, &game_);
    } else if (game_.getPlayerOnTurn() == bot) {
        bot->play(this, &game_);
        finishRound();
    }
}

string GameServer::playCard(Player * player, int position, int target, int target_card) {
    string card_name = player->getCards()[position]->getOriginalName();
    int status = game_.playCard(player, position, target, target_card);
    string res = to_string(status);
    if (status == Game::SUCCESS) {
        sendEvent("PLAY_CARD|" + player->getName() + "|" + card_name + "|" + to_string(target) + "|" + to_string(target_card));
    }
    for (Player * player : game_.getPendingPlayers()) {
        Bot * bot = dynamic_cast<Bot *>(player);
        if (bot) {
            startBot(bot);
        }
    }
    return res;
}

string GameServer::proceed(Player * player) {
    string res;
    if (player->getLife() > 0) {
        game_.proceed(player);
        sendEvent("PROCEED|" + player->getName());
        res = to_string(Game::SUCCESS);

        if (player->getLife() <= 0 && player->hasBeerCard()) {
            player->setPending(true);
        }
    } else {
        if (player->hasBeerCard()) {
            res = to_string(Game::ERROR_BEER_AVAILABLE);
        } else {
            res = to_string(Game::ERROR_UNKNOWN);
        }
    }
    return res;
}

string GameServer::finishRound() {
    auto winners = game_.getWinners();
    if (winners.size() > 0) {
        string roles;
        for (unsigned int i = 0; i < winners.size(); i++) {
            roles += winners[i];
            if (i < winners.size() - 1) {
                roles += ";";
            }
        }
        sendEvent("GAME_OVER|" + roles);
    } else {
        game_.finishRound();
        game_.drawCard(game_.getPlayerOnTurn());
        game_.drawCard(game_.getPlayerOnTurn());
        sendEvent("NEXT_ROUND");

        Bot * bot = dynamic_cast<Bot *>(game_.getPlayerOnTurn());
        if (bot) {
            startBot(bot);
        }
    }
    return GameServer::SUCCESS;
}

void GameServer::sendResponse(int client_socket, string res) {
    // send response
    if (send(client_socket, res.c_str(), res.size(), 0) == -1) {
        throw Exception(ERROR_SEND, strerror(errno));
    }
}

void GameServer::sendEvent(string event) {
    // cout << "sendEvent: " << event << endl;
    for (unsigned int i = 0; i < stream_connections_.size(); i++) {
        sendResponse(stream_connections_[i], event + '$');
    }
}

struct addrinfo * GameServer::getServerInfo() {
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, to_string(GameServer::PORT).c_str(), &hints, &res) != 0) {
        throw Exception(ERROR_GETADDRINFO, strerror(errno));
    }

    return res;
}

int GameServer::getSocket(struct addrinfo * servinfo) {
    int socket_fd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (socket_fd == -1) {
        throw Exception(ERROR_SOCKET, strerror(errno));
    }
    return socket_fd;
}

void GameServer::stop() {
    if (!running_) {
        throw Exception(ERROR_NOT_RUNNING, "Server is not running.");
    }

    running_ = false;
    close(socket_);
}
