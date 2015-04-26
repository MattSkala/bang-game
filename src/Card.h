#include <string>

using namespace std;


#ifndef BANG_ABSTRACTCARD_H
#define BANG_ABSTRACTCARD_H


class Card {
protected:
    string name_;
public:
    virtual void play() const = 0;
    string getName() const;
};


#endif //BANG_ABSTRACTCARD_H
