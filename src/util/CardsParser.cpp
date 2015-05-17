#include <cstdlib>
#include <fstream>
#include <vector>
#include <memory>
#include "CardsParser.h"
#include "CsvParser.h"
#include "../entity/Card.h"
#include "../entity/RoleCard.h"
#include "../entity/InstantCard.h"
#include "../entity/PermanentCard.h"
#include "../entity/GunCard.h"
#include "../entity/CharacterCard.h"


vector<Card *> CardsParser::parseFile(string filename) {
    CsvParser parser;
    vector< vector<string> > data = parser.parseFile(filename, true);

    vector<Card *> cards;
    for (vector<string>& row : data) {
        Card* card = createCard(row);
        if (card != NULL) {
            cards.push_back(card);
        }
    }
    return cards;
}

Card* CardsParser::createCard(vector<string> row) {
    string original_name = row[COL_ORIGINAL_NAME];
    string name = row[COL_NAME];
    string type = row[COL_TYPE];
    int count = stoi(row[COL_COUNT]);
    if (type == "role") {
        return new RoleCard(original_name, name, count);
    } else if (type == "character") {
        CharacterCard * card = new CharacterCard(original_name, name, count);
        card->setLife(stoi(row[COL_LIFE]));
        return card;
    } else if (type == "instant") {
        return new InstantCard(original_name, name, count);
    } else if (type == "permanent") {
        return new PermanentCard(original_name, name, count);
    } else if (type == "gun") {
        GunCard * card = new GunCard(original_name, name, count);
        int distance = stoi(row[COL_DISTANCE]);
        card->setDistance(distance);
        bool unlimited_bang = (row[COL_UNLIMITED_BANG] == "1");
        card->setUnlimitedBang(unlimited_bang);
        return card;
    } else {
        return NULL;
    }
}
