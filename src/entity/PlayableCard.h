#ifndef BANG_PLAYABLECARD_H
#define BANG_PLAYABLECARD_H


#include "Card.h"

class Game;
class Player;


/// A playable card.
class PlayableCard : public Card {
protected:

public:
    /**
     * Can target any player.
     */
    bool target_any_ = false;
    /**
     * Targets all other players.
     */
    bool target_others_ = false;
    /**
     * Can target a player within distance.
     */
    bool target_distance_ = false;
    /**
     * Targets the playing player.
     */
    bool target_self_ = false;

    PlayableCard(string original_name, string name, int count);

    void setTargetAny(bool target);
    void setTargetOthers(bool target);
    void setTargetDistance(bool target);
    void setTargetSelf(bool target);

    /// Returns true if card is targeting other player when being played.
    bool isTargetable();

    /// A card is played by player on target.
    virtual bool play(Game * game, Player * player, int position, int target) = 0;

    /// A card effect is accepted by target.
    virtual bool proceed(Game * game, Player * player);
};


#endif //BANG_PLAYABLECARD_H
