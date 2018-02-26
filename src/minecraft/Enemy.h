#ifndef ENEMY_H
#define ENEMY_H


#include "engine/world/GameObject.h"

class Enemy : public GameObject
{
public:
    Enemy(const std::string &name, const glm::vec3 &pos);
};

#endif // ENEMY_H
