#ifndef BANG_CARDSPARSER_H
#define BANG_CARDSPARSER_H


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
    static const int COL_UNLIMITED_BANG = 6;
    static const int COL_HEAL = 7;
    static const int COL_HEAL_OTHERS = 8;
    static const int COL_DRAW = 9;
    static const int COL_REMOVE_CARD = 10;
    Card * createCard(vector<string> row);
public:
    vector<Card *> parseFile(string filename);
};


#endif //BANG_CARDSPARSER_H
