#include "../src/util/CardsParser.h"
#include "../src/entity/Card.h"
#include "../src/entity/RoleCard.h"
#include "../src/entity/CharacterCard.h"
#include "../src/entity/GunCard.h"

void testCardsParser() {
    CardsParser parser;
    vector<Card *> cards = parser.parseFile("res/cards.csv");

    assert(cards.size() == 42 - 6);
    RoleCard * role = (RoleCard *) cards[0];
    assert(role->getOriginalName() == "Sceriffo");
    assert(role->getName() == "Šerif");

    CharacterCard * character = (CharacterCard *) cards[4];
    assert(character->getOriginalName() == "Bart Cassidy");
    assert(character->getName() == "Bart Cassidy");
    assert(character->getLife() == 4);

    GunCard * volcanic = (GunCard *) cards[31];
    assert(volcanic->getOriginalName() == "Volcanic");
    assert(volcanic->getDistance() == 1);
    assert(volcanic->hasUnlimitedBang());

    for (Card *card : cards) {
        delete card;
    }
}
