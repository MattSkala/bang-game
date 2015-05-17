#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <string>
#include <string.h>
#include <iostream>
#include <sstream>
#include "GameClient.h"
#include "../util/utils.h"

using namespace std;

#define BUFFER_SIZE 1024

GameClient::GameClient() {
    main_thread_id_ = this_thread::get_id();
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
    close(api_socket_);
    close(stream_socket_);
    api_socket_ = 0;
    stream_socket_ = 0;
}

void GameClient::sendRequest(string req, int socket) {
    send(socket, req.c_str(), req.size(), 0);
}

void GameClient::sendRequest(string req) {
    sendRequest(req, api_socket_);
}

bool GameClient::receiveResponse(string & res, int socket) {
    char buffer[BUFFER_SIZE];
    int received = 0;

    if ((received = recv(socket, buffer, BUFFER_SIZE, 0)) == -1) {
        perror(strerror(errno));
        throw "recv error";
    }

    if (received == 0) {
        return false;
    }

    res = string(buffer, received);

    return true;
}

bool GameClient::receiveResponse(string &res) {
    return receiveResponse(res, api_socket_);
}

void GameClient::stream(string host, int port) {
    try {
        stream_socket_ = connectSocket(host, port);
        sendRequest("SUBSCRIBE", stream_socket_);
        string res;
        bool status = receiveResponse(res, stream_socket_);
        if (!status || res != "OK") {
            throw "stream subscription failed";
        }

        while (stream_socket_ > 0) {
            string res;

            if (!receiveResponse(res, stream_socket_)) {
                throw "disconnect";
            }

            vector<string> parts = explode(res, '|');

            if (this_thread::get_id() == main_thread_id_) {
                last_response_ = res;
                stream_thread_.detach();
            } else {
                // notify listeners
                for (unsigned int i = 0; i < listeners_.size(); i++) {
                    listeners_[i](parts);
                }
            }
        }
    } catch (const char *err) {
        cerr << "stream error:" << err << endl;
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
    sendRequest("JOIN|" + username);
    string res;
    receiveResponse(res);
    return res == "OK";
}

void GameClient::addBot() {
    sendRequest("ADD_BOT");
}

vector<string> GameClient::getPlayers() {
    sendRequest("GET_PLAYERS");
    string res;
    receiveResponse(res);
    return explode(res, ';');
}
