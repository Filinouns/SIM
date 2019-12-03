#pragma once
#include "ForceGenerator.h"
class Gravity : public ForceGenerator {
	Vector3 gravity = {0, 10, 0};
public: 
	Gravity(const Vector3& _g);
	virtual void updateForce(RigidBody* body, float t);
};