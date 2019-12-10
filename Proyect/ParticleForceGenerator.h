#pragma once
#include "Particula.h"
#include "PxRigidDynamic.h"
#include "RigidObject.h"
class ParticleForceGenerator
{
public:
	ParticleForceGenerator() {};
	virtual ~ParticleForceGenerator() {};
	virtual void updateForce(Particula* particle, float t) = 0;
	virtual void updateForce(RigidObject* body, float t) = 0;
};