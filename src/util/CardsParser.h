#ifndef BANG_CARDSPARSER_H
#define BANG_CARDSPARSER_H


#include "../Card.h"

/**
 * A helper class for reading cards configuration from CSV file.
 */
class CardsParser {
public:
    vector<Card *> parseFile(string filename);
};


#endif //BANG_CARDSPARSER_H
