#ifndef BANG_GAMECLIENT_H
#define BANG_GAMECLIENT_H


#include <string>
#include <thread>
#include <vector>
#include <map>
#include <mutex>
#include "../entity/Player.h"

using namespace std;


/// A game client for communication with GameServer.
class GameClient {
private:
    static const string ERROR_DISCONNECT;
    static const string ERROR_GETADDRINFO;
    static const string ERROR_SOCKET;
    static const string ERROR_CONNECT;
    static const string ERROR_RECV;
    static const string ERROR_SUBSCRIBE;

    int stream_socket_;
    int api_socket_;
    vector<function<bool(vector<string>)>> listeners_;
    thread stream_thread_;
    mutex lock_;

    /// Opens new socket and connects to server on provided port.
    int connectSocket(string host, int port);

    /**
     * \brief Starts a blocking recv loop through which a client is notified about game state changes.
     * Listeners for events can be registered with GameClient::addListener method.
     */
    void stream(string host, int port);

    /// Sends a request to game server through provided socket.
    void sendRequest(string req, int socket);

    /// Receives a response from the server through provided socket.
    bool receiveResponse(string & res, int socket);

    /// Sends a request to game server through API socket and receives a response. The call is thread-safe.
    bool sendRequest(string req, string & res);

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

    /// Checks whether there is an open socket connected to the server.
    bool isConnected();

    /// Adds a listener to be notified about changes in game state.
    void addListener(function<bool(vector<string>)> f);

    /// Removes a listener.
    /**
     * \param f The listener previously registered with GameClient::addListener.
     */
    bool removeListener(function<bool(vector<string>)> & f);

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
    vector<string> getPlayers();

    /// Gets a list of players with game info.
    /**
     * \return A vector of vectors containing players information (username, life points, character, role, on turn).
     */
    vector<vector<string>> getPlayersInfo();

    /// Starts the game.
    bool startGame();

    /// Gets a list of cards in hand.
    vector<string> getCards();

    /// Gets lists of laid permanent cards.
    map<string, vector<string>> getPermanentCards();

    /// Finishes current round.
    bool finishRound();

    /// Discards a card from hand.
    bool discardCard(int position);

    /// Plays a card from hand with effect on target player's laid card.
    int playCard(int position, int target = -1, int target_card = -1);

    /// Does not reply to pending card.
    int proceed();

    ///@}
};


#endif //BANG_GAMECLIENT_H
