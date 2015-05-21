#ifndef BANG_CARD_H
#define BANG_CARD_H


#include <string>

using namespace std;


/// An abstract card entity.
class Card {
private:
    /// The original name of the card.
    string original_name_;

    /// The translated name of the card.
    string name_;

    /// The number of cards in a pack.
    int count_;
public:
    Card(string original_name, string name, int count);

    virtual ~Card();

    string getOriginalName() const;

    string getName() const;

    int getCount() const;
};


#endif //BANG_CARD_H
