#include "Card.h"

Card::Card(string original_name, string name, int count) : original_name_(original_name), name_(name), count_(count) { }

Card::~Card() { }

string Card::getOriginalName() const {
    return original_name_;
}

string Card::getName() const {
    if (name_ == "") {
        return original_name_;
    }
    return name_;
}

int Card::getCount() const {
    return count_;
}
