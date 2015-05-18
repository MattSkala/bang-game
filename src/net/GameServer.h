#ifndef BANG_GAMESERVER_H
#define BANG_GAMESERVER_H


#include <map>
#include "../Game.h"


/// A game server which runs on host computer in a separate process.
/**
 * \section API
 *
 * Request | Response | Action
 * ------- | -------- | ------
 * SUBSCRIBE | OK | Subscribes to stream events updates.
 * JOIN\|username | OK | Joins the game with given username.
 * ADD_BOT | OK | Adds a bot player.
 * GET_PLAYERS | username1;username2;... | Returns a list of connected players.
 * START | OK | Starts the game.
 * GET_CARDS | originalname1;originalname2;... | Returns a list of cards in hand.
 * PLAY_CARD\|originalname[\|options...] | OK | Plays a card with optional options (e. g. target player).
 * DRAW_CARD | originalname | Draws a card from pack.
 * FINISH_ROUND | OK | Finishes current round.
 *
 *
 * \section Stream Events
 *
 * Players can be notified about game state changes by events.
 * They are being sent through stream socket when client subscribes with SUBSCRIBE request.
 *
 * Event | Description
 * ----- | -----------
 * JOIN\|username | A player joined the game.
 * LEAVE\|username | The player left the game.
 * START | The game was started by the host.
 * NEXT_ROUND\|username | The player starts his round.
 * PLAY_CARD\|username\|originalname[\|options...] | The card was played.
 */
class GameServer {
private:
    /**
     * Game state.
     */
    Game & game_;

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
    GameServer(Game & game);

    ~GameServer();

    /**
     * The port server is listening on.
     */
    static const int PORT = 7769;

    /**
     * Min count of players that should be connected before starting a game.
     */
    static const int MIN_PLAYERS = 2;

    /**
     * Max count of players that are able to join the game.
     */
    static const int MAX_PLAYERS = 7;

    /**
     * The response that is returned by server on response-less request.
     */
    static const string SUCCESS_RESPONSE;

    /// Starts the game server.
    /**
     * It should be called in a separate process as it starts a blocking loop.
     * The server listens on port GameServer::PORT and automatically accepts incoming connections.
     */
    void start();

    /// Stops the game server.
    void stop();
};


#endif //BANG_GAMESERVER_H
