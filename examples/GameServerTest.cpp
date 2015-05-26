#include "../src/net/GameServer.h"
#include "../src/net/GameClient.h"
#include "../src/Exception.h"
#include <unistd.h>
#include <signal.h>
#include <string>
#include <iostream>
#include <cassert>

using namespace std;

void testGameServer() {
    int pid = fork();
    if (pid == 0) {
        try {
            Game game;
            GameServer server(game);
            server.start();
        } catch (Exception err) {
            cout << "server error: " << err.getMessage() << endl;
        }
        exit(0);
    }

    // wait for server start
    usleep(100 * 1000);

    try {
        GameClient client;
        client.connect("127.0.0.1", GameServer::PORT);

        // GET_PLAYERS
        client.join("Matt");
        vector<string> players = client.getPlayers();
        assert(players.size() == 1);
        assert(players[0] == "Matt");

        client.disconnect();
    } catch (Exception err) {
        cout << "client error: " << err.getMessage() << endl;
    }

    kill(pid, SIGTERM);
}
