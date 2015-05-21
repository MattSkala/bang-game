#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include "GameClient.h"
#include "../util/utils.h"
#include "GameServer.h"

using namespace std;

#define BUFFER_SIZE 1024

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
        perror(gai_strerror(status));
        throw "getaddrinfo error";
    }

    if ((sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1) {
        perror(strerror(errno));
        throw "socket error";
    }

    if (::connect(sock, res->ai_addr, res->ai_addrlen) == -1) {
        perror(strerror(errno));
        throw "connect error";
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
            perror(strerror(errno));
            throw "recv error";
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
            throw "stream subscription failed";
        }

        while (stream_socket_ > 0) {
            string res;

            if (!receiveResponse(res, stream_socket_)) {
                throw "disconnect";
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
    } catch (const char *err) {
        if (isConnected()) {
            cerr << "stream error:" << err << endl;
        }
    }
}

void GameClient::addListener(function<bool(vector<string>)> f) {
    listeners_.push_back(f);
}

bool GameClient::removeListener(function<bool(vector<string>)> f) {
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
    return sendRequest("JOIN|" + username, res) && res == GameServer::SUCCESS;
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

bool GameClient::playCard(int position) {
    string res;
    return sendRequest("PLAY_CARD|" + to_string(position), res) && res == GameServer::SUCCESS;
}

bool GameClient::playCard(int position, int target) {
    string res;
    return sendRequest("PLAY_CARD|" + to_string(position) + "|" + to_string(target), res) && res == GameServer::SUCCESS;
}

bool GameClient::proceed() {
    string res;
    return sendRequest("PROCEED", res) && res == GameServer::SUCCESS;
}
