#include <string>
#include <thread>
#include <vector>

using namespace std;

#ifndef BANG_GAMECLIENT_H
#define BANG_GAMECLIENT_H


class GameClient {
private:
    int stream_socket_;
    int api_socket_;
    vector<function<bool(vector<string>)>> listeners_;
    thread stream_thread_;
    thread::id main_thread_id_;
    string last_response_;

    /**
     * Opens new socket and connects to server on provided port.
     */
    int connectSocket(string host, int port);

    /**
     * Starts a blocking recv loop through which a client is notified about game state changes.
     * Listeners for events can be registered with GameClient::addListener method.
     */
    void stream(string host, int port);

    /**
     * Sends a request to game server through provided socket.
     */
    void sendRequest(string req, int socket);

    /**
     * Receives a response from the server through provided socket.
     */
    bool receiveResponse(string & res, int socket);
public:
    GameClient();

    ~GameClient();

    /**
     * Opens new stream connection and starts GameClient::stream in a new detached thread.
     */
    void connectStream(string host, int port);

    /**
     * Opens new API connection.
     */
    void connectApi(string host, int port);

    /**
     * Opens both stream and API connection.
     */
    void connect(string host, int port);

    /**
     * Disconnects from the game server.
     */
    void disconnect();

    /**
     * Sends a request to game server through API socket.
     */
    void sendRequest(string req);

    /**
     * Receives a response from the server through API socket.
     */
    bool receiveResponse(string & res);

    /**
     * Adds a listener to be notified about changes in game state.
     */
    void addListener(function<bool(vector<string>)> f);

    /**
     * Removes a listener previously registered with GameClient::addListener.
     */
    bool removeListener(function<bool(vector<string>)> f);

    /**
     * Joins a game.
     */
    bool join(string username);

    /**
     * Adds a bot player.
     */
    void addBot();

    /**
     * Gets connected players.
     */
    vector<string> getPlayers();
};


#endif //BANG_GAMECLIENT_H
