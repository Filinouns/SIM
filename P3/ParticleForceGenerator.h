#pragma once
#include "Particle.h"

class ParticleForceGenerator
{
public:
	//ParticleForceGenerator();
	//virtual ~ParticleForceGenerator();

	virtual void updateForce(Particle* particle, float t) = 0;
};