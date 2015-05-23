#include <unistd.h>
#include <signal.h>
#include <string>
#include <iostream>
#include "../src/Game.h"
#include "../src/entity/PermanentCard.h"
#include "../src/entity/GunCard.h"
#include "../src/entity/BangCard.h"
#include "../src/entity/DiscardCard.h"
#include "../src/entity/DrawCard.h"

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
    a->setLife(2);
    b->setLife(2);
    c->setLife(2);
    d->setLife(2);

    assert(game.getPlayers().size() == 4);
    assert(game.getDistance(a, 1) == 1);
    assert(game.getDistance(a, 2) == 2);
    assert(game.getDistance(a, 3) == 1);

    b->setLife(0);

    assert(game.getPlayers().size() == 4);
    assert(game.getDistance(a, 2) == 1);
    assert(game.getDistance(a, 3) == 1);

    b->setLife(2);

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

void testGameUnlimitedBang() {
    Game game;
    Player * a = new Player();
    Player * b = new Player();
    game.addPlayer(a);
    game.addPlayer(b);

    shared_ptr<BangCard> bang = shared_ptr<BangCard>(new BangCard("Bang!", "", 2));
    bang->setTargetDistance(true);
    shared_ptr<GunCard> volcanic = shared_ptr<GunCard>(new GunCard("Volcanic", "", 1, 1));
    volcanic->setUnlimitedBang(true);
    a->addCard(bang);
    a->addCard(bang);
    a->addCard(volcanic);
    a->layCard(2);

    assert(game.playCard(a, 0, 1) == Game::SUCCESS);
    assert(game.playCard(a, 0, 1) == Game::SUCCESS);
}

void testGameDiscardCard() {
    Game game;
    Player * a = new Player();
    Player * b = new Player();
    game.addPlayer(a);
    game.addPlayer(b);

    shared_ptr<GunCard> volcanic = shared_ptr<GunCard>(new GunCard("Volcanic", "", 1, 1));
    shared_ptr<DiscardCard> discard = shared_ptr<DiscardCard>(new DiscardCard("Cat Balou", "", 1));
    discard->setTargetAny(true);

    a->addCard(discard);
    b->addCard(volcanic);
    b->layCard(0);
    assert(b->getPermanentCards().size() == 1);
    assert(a->getCards().size() == 1);
    assert(game.playCard(a, 0, 1, 0) == Game::SUCCESS);
    assert(b->getPermanentCards().size() == 0);
    assert(a->getCards().size() == 0);
}

void testGameDrawCard() {
    Game game;
    Player * a = new Player();
    Player * b = new Player();
    game.addPlayer(a);
    game.addPlayer(b);

    shared_ptr<GunCard> volcanic = shared_ptr<GunCard>(new GunCard("Volcanic", "", 1, 1));
    shared_ptr<DrawCard> draw = shared_ptr<DrawCard>(new DrawCard("Panic", "", 1, 1));
    draw->setTargetDistance(true);

    a->addCard(draw);
    b->addCard(volcanic);
    b->layCard(0);
    assert(b->getPermanentCards().size() == 1);
    assert(a->getCards().size() == 1);
    assert(game.playCard(a, 0, 1, 0) == Game::SUCCESS);
    assert(b->getPermanentCards().size() == 0);
    assert(a->getCards().size() == 1);
}

void testGameOver() {
    Game game;
    Player * a = new Player();
    Player * b = new Player();
    Player * c = new Player();
    Player * d = new Player();
    game.addPlayer(a);
    game.addPlayer(b);
    game.addPlayer(c);
    game.addPlayer(d);

    RoleCard * sheriff = new RoleCard(RoleCard::SHERIFF, "", 1);
    RoleCard * renegate = new RoleCard(RoleCard::RENEGATE, "", 1);
    RoleCard * outlaw = new RoleCard(RoleCard::OUTLAW, "", 2);
    CharacterCard * character = new CharacterCard("Paul Regret", "", 1);
    character->setLife(3);

    a->setRole(sheriff);
    b->setRole(renegate);
    c->setRole(outlaw);
    d->setRole(outlaw);
    a->setCharacter(character);
    b->setCharacter(character);
    c->setCharacter(character);
    d->setCharacter(character);
    a->setLife(a->getMaxLife());
    b->setLife(b->getMaxLife());
    c->setLife(c->getMaxLife());
    d->setLife(d->getMaxLife());

    assert(game.getWinners().size() == 0);
    a->setLife(0);
    assert(game.getWinners().size() == 1);
    assert(game.getWinners()[0] == RoleCard::OUTLAW);
    c->setLife(0);
    d->setLife(0);
    assert(game.getWinners().size() == 1);
    assert(game.getWinners()[0] == RoleCard::RENEGATE);

    delete sheriff;
    delete renegate;
    delete outlaw;
    delete character;
}

void testGame() {
    testGameDistance();
    testGameUnlimitedBang();
    testGameDiscardCard();
    testGameDrawCard();
    testGameOver();
}
