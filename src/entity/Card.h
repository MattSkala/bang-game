#ifndef BANG_CARD_H
#define BANG_CARD_H


#include <string>

using namespace std;


class Game;

class Card {
protected:
    string original_name_;
    string name_;
    int count_;
public:
    Card(string original_name, string name, int count);
    virtual ~Card();
    virtual void play(Game & game) const;
    string getOriginalName() const;
    string getName() const;
    int getCount() const;
};


#endif //BANG_CARD_H
