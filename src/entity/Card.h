#ifndef BANG_CARD_H
#define BANG_CARD_H


#include <string>

using namespace std;


class Game;

/// An abstract card entity.
class Card {
protected:
    /// The original name of card.
    string original_name_;

    /// The translated name of card.
    string name_;

    /// The number of cards in a pack.
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
