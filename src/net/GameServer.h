#ifndef BANG_GAMESERVER_H
#define BANG_GAMESERVER_H


#include <map>
#include <thread>
#include "../Game.h"
#include "../entity/Bot.h"


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
 * GET_PLAYERS_INFO | username,life,character,role,on_turn,pending,cards;... | Returns a list of players with game info.
 * START | OK | Starts the game.
 * GET_CARDS | originalname1;originalname2;... | Returns a list of cards in hand.
 * GET_PERMANENT_CARDS | username1:originalname1,originalname2;... | Returns lists of laid permanent cards.
 * PLAY_CARD\|position\|target\|target_card | code | Plays a card from hand with optional target.
 * DISCARD_CARD\|position | OK |  Discards a card from hand.
 * FINISH_ROUND | OK | Finishes current round.
 * PROCEED | code | Does not reply to pending card.
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
 * NEXT_ROUND | The next player is on turn.
 * PROCEED | The player does not reply to pending card.
 * PLAY_CARD\|username\|originalname[\|options...] | The card was played.
 * GAME_OVER\|role1;role2;... | The game has ended. Returns winning roles.
 */
class GameServer {
private:
    static const string ERROR_ALREADY_RUNNING;
    static const string ERROR_NOT_RUNNING;
    static const string ERROR_SETSOCKOPT;
    static const string ERROR_BIND;
    static const string ERROR_LISTEN;
    static const string ERROR_ACCEPT;
    static const string ERROR_RECV;
    static const string ERROR_SEND;
    static const string ERROR_SUBSCRIBE;
    static const string ERROR_GETADDRINFO;
    static const string ERROR_SOCKET;

    /// Game state.
    Game & game_;

    /// True if the server is running.
    bool running_ = false;

    /// A listenable server socket.
    int socket_;

    /// All active socket connections.
    vector<int> connections_;

    /// Socket connections subscribed to stream.
    vector<int> stream_connections_;

    /// An API socket to player map.
    map<int, Player*> players_;

    /// Gets address info for localhost and server port.
    struct addrinfo * getServerInfo();

    /// Returns socket descriptor for provided address.
    int getSocket(struct addrinfo * servinfo);

    void waitForConnection();

    void acceptNewConnection();

    bool receiveRequest(int client_socket, string & req);

    string processRequest(string req, int connection);

    void sendResponse(int client_socket, string res);

    void handleUserLeave(vector<int>::iterator it);

    /// Sends an event notification to all connected clients.
    void sendEvent(string event);

    void startBot(Bot * bot);

    void handleBot(Bot * bot);
public:
    GameServer(Game & game);

    ~GameServer();

    /// The port server is listening on.
    static const int PORT = 7768;

    /// Min count of players that should be connected before starting a game.
    static const int MIN_PLAYERS = 4;

    /// Max count of players that are able to join the game.
    static const int MAX_PLAYERS = 7;

    /// Max count of bots.
    static const int MAX_BOTS = 3;

    /// The response that is returned by server on successful response-less request.
    static const string SUCCESS;

    /// The response is returned by server on error.
    static const string ERROR;

    /// Joining with duplicate name.
    static const string ERROR_JOIN_NAME;

    /// Joining when the game already started.
    static const string ERROR_JOIN_PLAYING;

    /// Bot limit is reached.
    static const string ERROR_BOT_LIMIT;

    static const string REQ_SUBSCRIBE;
    static const string REQ_JOIN;
    static const string REQ_ADD_BOT;
    static const string REQ_GET_PLAYERS;
    static const string REQ_GET_PLAYERS_INFO;
    static const string REQ_START;
    static const string REQ_GET_CARDS;
    static const string REQ_GET_PERMANENT_CARDS;
    static const string REQ_PLAY_CARD;
    static const string REQ_DISCARD_CARD;
    static const string REQ_FINISH_ROUND;
    static const string REQ_PROCEED;

    static const string EVENT_JOIN;
    static const string EVENT_LEAVE;
    static const string EVENT_START;
    static const string EVENT_NEXT_ROUND;
    static const string EVENT_PROCEED;
    static const string EVENT_PLAY_CARD;
    static const string EVENT_GAME_OVER;

    /// Starts the game server.
    /**
     * It should be called in a separate process as it starts a blocking loop.
     * The server listens on port GameServer::PORT and automatically accepts incoming connections.
     */
    void start();

    string playCard(Player * player, int position, int target, int target_card);

    string proceed(Player * player);

    string finishRound();

    /// Stops the game server.
    void stop();
};


#endif //BANG_GAMESERVER_H
