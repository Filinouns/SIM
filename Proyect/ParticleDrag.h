#pragma once
#include "ParticleForceGenerator.h"
class ParticleDrag :
	public ParticleForceGenerator
{
public:
	ParticleDrag(float _k1, float _k2): k1(_k1), k2(_k2){};
	virtual ~ParticleDrag() {};

	virtual void updateForce(Particula* particle, float t);

private:
	float k1; // Coefficient for velocity
	float k2; // Coefficient for squared velocity
};