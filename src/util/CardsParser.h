#ifndef BANG_CARDSPARSER_H
#define BANG_CARDSPARSER_H


#include <vector>
#include "../entity/Card.h"


/// A helper class for reading cards configuration from CSV file.
class CardsParser {
private:
    static const int COL_ORIGINAL_NAME = 0;
    static const int COL_NAME = 1;
    static const int COL_COUNT = 2;
    static const int COL_TYPE = 3;
    static const int COL_LIFE = 4;
    static const int COL_DISTANCE = 5;
    static const int COL_MISSED = 6;
    static const int COL_BANG = 7;
    static const int COL_HEAL = 8;
    static const int COL_DRAW = 9;
    static const int COL_DISCARD = 10;
    static const int COL_TARGET_ANY = 11;
    static const int COL_TARGET_OTHERS = 12;
    static const int COL_TARGET_DISTANCE = 13;
    static const int COL_TARGET_SELF = 14;
    static const int COL_SPECIAL = 15;
    static const int COL_UNLIMITED_BANG = 16;
    Card * createCard(vector<string> row);
public:
    vector<Card *> parseFile(string filename);
};


#endif //BANG_CARDSPARSER_H
