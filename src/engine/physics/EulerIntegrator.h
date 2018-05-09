#ifndef EULERINTEGRATOR_H
#define EULERINTEGRATOR_H

#include "PhysicsIntegrator.h"
#include <memory>

class RigidBody;

class EulerIntegrator : public PhysicsIntegrator {
private:
    typedef std::vector<std::shared_ptr<RigidBody>> ProcItemsCont;
	ProcItemsCont procItems;

public:
    static void integrateBodies(std::vector<std::shared_ptr<RigidBody> > &bodies, float dt);

};

#endif // EULERINTEGRATOR_H
