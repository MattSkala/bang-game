#include "../src/net/GameServer.h"
#include "../src/net/GameClient.h"
#include <unistd.h>
#include <signal.h>
#include <string>

void testGameServer() {
    int pid = fork();
    if (pid == 0) {
        try {
            GameServer server;
            server.start();
        } catch (const char *err) {
            cout << "server error:" << err << endl;
        }
        exit(0);
    }

    // wait for server start
    usleep(100 * 1000);

    try {
        GameClient client;
        client.connectApi("127.0.0.1", GameServer::PORT);

        // PING
        client.sendRequest("PING");
        string res;
        bool status = client.receiveResponse(res);
        assert(status);
        assert(res == "PONG");

        // GET_PLAYERS
        client.join("Matt");
        vector<string> players = client.getPlayers();
        assert(players.size() == 1);
        assert(players[0] == "Matt");

        client.disconnect();
    } catch (const char *err) {
        cout << "client error:" << err << endl;
    }

    kill(pid, SIGTERM);
}
