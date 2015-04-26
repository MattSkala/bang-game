#ifndef BANG_PLAYER_H
#define BANG_PLAYER_H

#include <string>

using namespace std;

class Player {
private:
    int max_life_;
    int life_;
    string name_;
public:
    Player();
    void setName(string name);
    string getName() const;
    /**
     * Decreases life points by one.
     */
    void decreaseLife();
    /**
     * Heals one life point.
     * @return True if a life point was healed or false if user already has maximal life points count.
     */
    bool increaseLife();
    /**
     * Returns current life points count.
     */
    int getLife() const;
};


#endif //BANG_PLAYER_H
