#ifndef PLAYER_H
#define PLAYER_H

#include "engine/world/GameObject.h"

class Player : public GameObject
{
public:
    Player(const std::string &name);
};

#endif // PLAYER_H
