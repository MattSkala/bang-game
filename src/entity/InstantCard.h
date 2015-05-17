#ifndef BANG_INSTANTCARD_H
#define BANG_INSTANTCARD_H


#include "Card.h"


class InstantCard : public Card {
public:
    InstantCard(string original_name, string name, int count);
};


#endif //BANG_INSTANTCARD_H
