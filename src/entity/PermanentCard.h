#ifndef BANG_PERMANENTCARD_H
#define BANG_PERMANENTCARD_H


#include "Card.h"


/// A playable card which can be laid on table and has a permanent effect until it is removed.
class PermanentCard : public Card {
public:
    PermanentCard(std::string original_name, std::string name, int count);
};


#endif //BANG_PERMANENTCARD_H
