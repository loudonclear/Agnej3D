#ifndef PLAYER_H
#define PLAYER_H


#include "engine/world/GameObject.h"

class OBJCollisionSystem;

class Player : public GameObject
{
public:
    Player(const std::string &name, std::shared_ptr<OBJCollisionSystem> objc);
};

#endif // PLAYER_H
