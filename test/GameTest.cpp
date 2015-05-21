#include "../src/Game.h"
#include "../src/entity/PermanentCard.h"
#include <unistd.h>
#include <signal.h>
#include <string>
#include <iostream>

using namespace std;

void testGameDistance() {
    Game game;
    Player * a = new Player();
    Player * b = new Player();
    Player * c = new Player();
    Player * d = new Player();
    game.addPlayer(a);
    game.addPlayer(b);
    game.addPlayer(c);
    game.addPlayer(d);

    assert(game.getPlayers().size() == 4);
    assert(game.getDistance(a, 1) == 1);
    assert(game.getDistance(a, 2) == 2);
    assert(game.getDistance(a, 3) == 1);

    shared_ptr<PermanentCard> appalossa = shared_ptr<PermanentCard>(new PermanentCard("Appalossa", "", 1));
    appalossa->setDistanceTweak(-1);
    a->addCard(appalossa);
    a->layCard(0);

    assert(game.getDistance(a, 1) == 1);
    assert(game.getDistance(a, 2) == 1);
    assert(game.getDistance(a, 3) == 1);
    assert(game.getDistance(b, 0) == 1);
    assert(game.getDistance(c, 0) == 2);
    assert(game.getDistance(d, 0) == 1);

    shared_ptr<PermanentCard> mustang = shared_ptr<PermanentCard>(new PermanentCard("Mustang", "", 1));
    mustang->setDistanceTweak(1);
    b->addCard(mustang);
    b->layCard(0);

    assert(game.getDistance(c, 1) == 2);
    assert(game.getDistance(a, 1) == 1);
}

void testGame() {
    testGameDistance();
}
