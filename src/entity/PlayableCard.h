#ifndef BANG_PLAYABLECARD_H
#define BANG_PLAYABLECARD_H


#include "Card.h"

class Game;
class Player;


/// A playable card.
class PlayableCard : public Card {
protected:
    /// Can target any player.
    bool target_any_ = false;

    /// Targets all other players.
    bool target_others_ = false;

    /// Can target a player within distance.
    bool target_distance_ = false;

    /// Targets the playing player.
    bool target_self_ = false;
public:
    PlayableCard(string original_name, string name, int count);

    void setTargetAny(bool target);
    void setTargetOthers(bool target);
    void setTargetDistance(bool target);
    void setTargetSelf(bool target);

    /// Returns true if card is targeting other player when being played.
    bool isTargetable() const;

    /// Returns true if card can target specific laid card.
    virtual bool isCardTargetable() const;

    /// A card is played by player on target.
    virtual int play(Game *game, Player *player, int position, int target, int target_card) = 0;

    /// A card effect is accepted by target.
    virtual bool proceed(Game * game, Player * player);
};


#endif //BANG_PLAYABLECARD_H
