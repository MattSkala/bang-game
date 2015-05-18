#include <string>
#include <thread>
#include <vector>

using namespace std;

#ifndef BANG_GAMECLIENT_H
#define BANG_GAMECLIENT_H


/// A game client for communication with GameServer.
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

    /// Sends a request to game server through API socket.
    /**
     * GameClient::receiveResponse should be called immediately after to obtain the response.
     */
    void sendRequest(string req);

    /// Receives a response from the server through API socket.
    bool receiveResponse(string & res);

    /// Opens new stream connection and starts GameClient::stream in a new detached thread.
    void connectStream(string host, int port);

    /// Opens new API connection.
    void connectApi(string host, int port);
public:
    GameClient();

    ~GameClient();

    /// Opens both stream and API connection.
    /**
     * \param host IP address of the server.
     * \param port The port that server is listening on.
     */
    void connect(string host, int port);

    /// Disconnects from the game server.
    void disconnect();

    /// Adds a listener to be notified about changes in game state.
    /**
     * \param f A listener to be notified about game state changes.
     */
    void addListener(function<bool(vector<string>)> f);

    /// Removes a listener.
    /**
     * \param f The listener previously registered with GameClient::addListener.
     */
    bool removeListener(function<bool(vector<string>)> f);

    ///@{
    /// \name API Requests

    /// Joins the game.
    /**
     * \param username The username to join the game with.
     * \return True if join was successful, false if server rejected the user.
     */
    bool join(string username);

    /// Adds a bot player.
    bool addBot();

    /// Gets a list of connected players.
    /**
     * \return A list of connected players.
     */
    vector<string> getPlayers();

    /// Starts the game.
    bool startGame();

    /// Gets a list of cards in hand.
    /**
     * \return A list of cards in hand.
     */
    vector<string> getCards();

    ///@}
};


#endif //BANG_GAMECLIENT_H
