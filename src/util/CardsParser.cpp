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
#include "../entity/BangCard.h"
#include "../entity/MissCard.h"
#include "../entity/BeerCard.h"
#include "../entity/DrawCard.h"

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
    bool miss = row[COL_MISSED] == "1";
    bool bang = row[COL_BANG] == "1";
    bool heal = row[COL_HEAL] == "1";
    int draw = row[COL_DRAW].size() > 0 ? stoi(row[COL_DRAW]) : 0;
    int discard = row[COL_DISCARD].size() > 0 ? stoi(row[COL_DISCARD]) : 0;
    bool target_any = row[COL_TARGET_ANY] == "1";
    bool target_others = row[COL_TARGET_OTHERS] == "1";
    bool target_distance = row[COL_TARGET_DISTANCE] == "1";
    bool target_self = row[COL_TARGET_SELF] == "1";
    bool special = row[COL_SPECIAL] == "1";
    if (type == "role") {
        return new RoleCard(original_name, name, count);
    } else if (type == "character") {
        CharacterCard * card = new CharacterCard(original_name, name, count);
        card->setLife(stoi(row[COL_LIFE]));
        return card;
    } else if (type == "instant") {
        InstantCard * card;
        if (bang) {
            card = new BangCard(original_name, name, count);
        } else if (miss) {
            card = new MissCard(original_name, name, count);
        } else if (heal) {
            card = new BeerCard(original_name, name, count);
        } else if (draw) {
            card = new DrawCard(original_name, name, count, draw);
        } else {
            card = new InstantCard(original_name, name, count);
        }
        card->setTargetAny(target_any);
        card->setTargetOthers(target_others);
        card->setTargetDistance(target_distance);
        card->setTargetSelf(target_self);
        return card;
    } else if (type == "permanent") {
        return new PermanentCard(original_name, name, count);
    } else if (type == "gun") {
        GunCard * card = new GunCard(original_name, name, count);
        int distance = stoi(row[COL_DISTANCE]);
        card->setDistance(distance);
        //bool unlimited_bang = (row[COL_UNLIMITED_BANG] == "1");
        //card->setUnlimitedBang(unlimited_bang);
        return card;
    } else {
        return NULL;
    }
}
