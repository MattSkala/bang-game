#ifndef BANG_PERMANENTCARD_H
#define BANG_PERMANENTCARD_H


#include "Card.h"


class PermanentCard : public Card {
public:
    PermanentCard(std::string original_name, std::string name, int count);
};


#endif //BANG_PERMANENTCARD_H
