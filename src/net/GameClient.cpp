#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include "GameServer.h"
#include "../Exception.h"
#include "../util/utils.h"
#include "GameClient.h"

using namespace std;

#define BUFFER_SIZE 1024

const string GameClient::ERROR_DISCONNECT = "DISCONNECT";
const string GameClient::ERROR_GETADDRINFO = "GETADDRINFO";
const string GameClient::ERROR_SOCKET = "SOCKET";
const string GameClient::ERROR_CONNECT = "CONNECT";
const string GameClient::ERROR_RECV = "RECV";
const string GameClient::ERROR_SUBSCRIBE = "SUBSCRIBE";


GameClient::GameClient() {
}

GameClient::~GameClient() {
    disconnect();
}

int GameClient::connectSocket(string host, int port) {
    struct addrinfo hints, *res;
    int sock;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status;
    if ((status = getaddrinfo(host.c_str(), to_string(port).c_str(), &hints, &res)) != 0) {
        throw Exception(ERROR_GETADDRINFO, gai_strerror(status));
    }

    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        throw Exception(ERROR_SOCKET, strerror(errno));
    }

    if (::connect(sock, res->ai_addr, res->ai_addrlen) == -1) {
        throw Exception(ERROR_CONNECT, strerror(errno));
    }

    freeaddrinfo(res);

    return sock;
}

void GameClient::connectStream(string host, int port) {
    stream_thread_ = thread(&GameClient::stream, this, host, port);
    stream_thread_.detach();
}

void GameClient::connectApi(string host, int port) {
    api_socket_ = connectSocket(host, port);
}

void GameClient::connect(string host, int port) {
    connectApi(host, port);
    connectStream(host, port);
}

void GameClient::disconnect() {
    int api_socket = api_socket_;
    int stream_socket = stream_socket_;
    api_socket_ = 0;
    stream_socket_ = 0;
    close(api_socket);
    close(stream_socket);
    if (stream_thread_.joinable()) {
        stream_thread_.join();
    }
}

bool GameClient::isConnected() {
    return api_socket_ > 0 || stream_socket_ > 0;
}

void GameClient::sendRequest(string req, int socket) {
    send(socket, req.c_str(), req.size(), 0);
}

bool GameClient::sendRequest(string req, string & res) {
    lock_.lock();
    sendRequest(req, api_socket_);
    int status = receiveResponse(res, api_socket_);
    lock_.unlock();
    return status;
}

bool GameClient::receiveResponse(string & res, int socket) {
    char buffer[BUFFER_SIZE];
    int received = 0;

    if ((received = recv(socket, buffer, BUFFER_SIZE, 0)) == -1) {
        if (isConnected()) {
            throw Exception(ERROR_RECV, strerror(errno));
        } else {
            return false;
        }
    }

    if (received == 0) {
        return false;
    }

    res = string(buffer, received);

    return true;
}

void GameClient::stream(string host, int port) {
    try {
        stream_socket_ = connectSocket(host, port);
        sendRequest("SUBSCRIBE", stream_socket_);
        string res;
        bool status = receiveResponse(res, stream_socket_);

        if (!status || res != GameServer::SUCCESS) {
            throw Exception(ERROR_SUBSCRIBE, "Cannot subscribe to stream server.");
        }

        while (stream_socket_ > 0) {
            string res;

            if (!receiveResponse(res, stream_socket_)) {
                throw Exception(ERROR_DISCONNECT, "Server disconnected.");
            }

            vector<string> events = explode(res, '$');
            for (unsigned int i = 0; i < events.size(); i++) {
                vector<string> parts = explode(events[i], '|');

                // notify listeners
                for (unsigned int i = 0; i < listeners_.size(); i++) {
                    listeners_[i](parts);
                }
            }
        }
    } catch (Exception err) {
        if (isConnected()) {
            cerr << err.getMessage() << endl;
        }
    }
}

void GameClient::addListener(function<bool(vector<string>)> f) {
    listeners_.push_back(f);
}

bool GameClient::removeListener(function<bool(vector<string>)> & f) {
    for (unsigned int i = 0; i < listeners_.size(); i++) {
        if (listeners_[i].target<bool(vector<string>)>() == f.target<bool(vector<string>)>()) {
            listeners_.erase(listeners_.begin() + i);
            return true;
        }
    }
    return false;
}

bool GameClient::join(string username) {
    string res;
    if (sendRequest("JOIN|" + username, res)) {
        if (res == GameServer::SUCCESS) {
            return true;
        } else if (res == GameServer::ERROR_JOIN_NAME) {
            throw Exception(GameServer::ERROR_JOIN_NAME, "Hráč s tímto jménem již existuje.");
        } else if (res == GameServer::ERROR_JOIN_PLAYING) {
            throw Exception(GameServer::ERROR_JOIN_PLAYING, "Hra byla již spuštěna.");
        }
        return false;
    }
    return false;
}

bool GameClient::addBot() {
    string res;
    return sendRequest("ADD_BOT", res) && res == GameServer::SUCCESS;
}

vector<string> GameClient::getPlayers() {
    string res;
    sendRequest("GET_PLAYERS", res);
    return explode(res, ';');
}

vector<vector<string>> GameClient::getPlayersInfo() {
    string res;
    sendRequest("GET_PLAYERS_INFO", res);
    vector<string> users = explode(res, ';');
    vector<vector<string>> players;
    for (string user : users) {
        vector<string> info = explode(user, ',');
        players.push_back(info);
    }
    return players;
}

bool GameClient::startGame() {
    string res;
    return sendRequest("START", res) && res == GameServer::SUCCESS;
}

vector<string> GameClient::getCards() {
    string res;
    sendRequest("GET_CARDS", res);
    return explode(res, ';');
}

map<string, vector<string>> GameClient::getPermanentCards() {
    map<string, vector<string>> players;
    string res;
    sendRequest("GET_PERMANENT_CARDS", res);
    vector<string> players_list = explode(res, ';');
    for (string player : players_list) {
        vector<string> player_parts = explode(player, ':');
        string name = player_parts[0];
        vector<string> cards;
        if (player_parts.size() == 2) {
            cards = explode(player_parts[1], ',');
        }
        players[name] = cards;
    }
    return players;
}

bool GameClient::finishRound() {
    string res;
    return sendRequest("FINISH_ROUND", res) && res == GameServer::SUCCESS;
}

bool GameClient::discardCard(int position) {
    string res;
    return sendRequest("DISCARD_CARD|" + to_string(position), res) && res == GameServer::SUCCESS;
}

int GameClient::playCard(int position, int target, int target_card) {
    string res;
    int status = sendRequest("PLAY_CARD|" + to_string(position) + "|" + to_string(target) + "|" + to_string(target_card), res);
    if (!status) {
        return Game::ERROR_UNKNOWN;
    }
    return stoi(res);
}

int GameClient::proceed() {
    string res;
    if (!sendRequest("PROCEED", res)) {
        return Game::ERROR_UNKNOWN;
    }
    return stoi(res);
}
