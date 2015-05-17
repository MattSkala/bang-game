#ifndef BANG_GAMESERVER_H
#define BANG_GAMESERVER_H


#include "../Game.h"
#include <map>

/**
 * Requests:
 * SUBSCRIBE // subscribes to stream events updates
 * JOIN|username -> OK // joins the game with given username
 * ADD_BOT -> OK // adds bot player
 * GET_PLAYERS -> username1;username2;... // returns a semicolon-separated list of connected players
 * GET_CARDS -> originalname1;originalname2;... // returns a semicolon-separated list of cards in hand
 * PLAY_CARD|originalname[|options...] -> OK // plays a card with optional options (e. g. target player)
 * DRAW_CARD -> originalname // draws a card from pack
 * FINISH_ROUND -> OK // finishes current round
 *
 * Stream Events:
 * JOIN|username // user joined the game
 * LEAVE|username // user left the game
 * START // the game was started by the host
 * NEXT_ROUND|username // the player starts his round
 * PLAY_CARD|username|originalname[|options...] // the card was player by user
 */
class GameServer {
private:
    /**
     * Game state.
     */
    Game game_;

    /**
     * True if the server is running.
     */
    bool running_ = false;

    /**
     * A listenable server socket.
     */
    int socket_;

    /**
     * All active socket connections.
     */
    vector<int> connections_;

    /**
     * Socket connections subscribed to stream.
     */
    vector<int> stream_connections_;

    /**
     * An API socket to player map.
     */
    map<int, Player*> players_;

    /**
     * Gets address info for localhost and server port.
     */
    struct addrinfo * getServerInfo();

    /**
     * Returns socket descriptor for provided address.
     */
    int getSocket(struct addrinfo * servinfo);
    void waitForConnection();
    void acceptNewConnection();
    bool receiveRequest(int client_socket, string & req);
    string processRequest(string req, int connection);
    void sendResponse(int client_socket, string res);
    void handleUserLeave(vector<int>::iterator it);

    /**
     * Sends an event notification to all connected clients.
     */
    void sendEvent(string event);
public:
    ~GameServer();

    /**
     * The port server is listening on.
     */
    static const int PORT = 7769;

    /**
     * Max count of players that are able to join the game.
     */
    static const int MAX_PLAYERS = 7;

    /**
     * Starts the game server. It should be called in a separate process as it starts a blocking loop.
     * The server listens on port GameServer::PORT and automatically accepts incoming connections.
     */
    void start();

    /**
     * Stops the game server.
     */
    void stop();
};


#endif //BANG_GAMESERVER_H
