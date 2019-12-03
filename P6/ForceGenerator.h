#pragma once

#include "RigidBody.h"

class ForceGenerator {
public:
	ForceGenerator();
	virtual ~ForceGenerator();
	virtual void updateForce(RigidBody* body, float t) = 0;
};