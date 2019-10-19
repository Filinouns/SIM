#pragma once
#include "ParticleForceGenerator.h"
class ParticleGravity :	public ParticleForceGenerator {

	// Acc para la gravedad
	Vector3 g;

public:
	ParticleGravity(const Vector3& gravity) : g(gravity) {}
	virtual ~ParticleGravity();

	virtual void updateForce(Particle* p, float t);
};