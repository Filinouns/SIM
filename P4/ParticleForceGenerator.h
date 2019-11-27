#pragma once
#include "Particle.h"

class ParticleForceGenerator {
public:
	//ParticleForceGenerator();
	//virtual ~ParticleForceGenerator();

	virtual void updateForce(Particle* particle, double t) = 0;
};