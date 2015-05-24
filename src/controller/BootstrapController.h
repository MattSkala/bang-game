#ifndef BANG_BOOTSTRAPCONTROLLER_H
#define BANG_BOOTSTRAPCONTROLLER_H


#include <string>
#include "Controller.h"
#include "../Game.h"
#include "../net/GameClient.h"

using namespace std;


/// A controller for welcome screen, creating a game and connecting players to it.
class BootstrapController : public Controller {
private:
    static const string LOCALHOST;
    Game & game_;
    GameClient & client_;
    pid_t & server_pid_;
    function<bool(vector<string>)> listener_;
    bool started_ = false;
    bool onStreamEvent(vector<string> event);
public:
    BootstrapController(Game & game, GameClient & client, pid_t & server_pid);
    void renderWelcome();
    string renderNameInput();
    void renderPlayersList();
    string renderServerInput();
    void actionHostGame();
    void actionAddBot();
    void actionJoinGame();
    bool actionStartGame();
};


#endif //BANG_BOOTSTRAPCONTROLLER_H
