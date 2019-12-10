#pragma once
#include "ParticleForceGenerator.h"
class ParticleGravity :
	public ParticleForceGenerator
{
public:
	ParticleGravity(const Vector3& gravity = {0, 0, 0}) : g(gravity) {};
	virtual ~ParticleGravity() {};

	Vector3 getGravity() {
		return g;
	}

	Vector3 setGravity(Vector3 grav) {
		g = grav;
	}

	virtual void updateForce(Particula* particle, float t);
	virtual void updateForce(RigidObject* particle, float t);
private:
	Vector3 g;
};