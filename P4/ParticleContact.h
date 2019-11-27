#pragma once

#include "Particle.h"

class ParticleContact {
public:
	ParticleContact();
	virtual ~ParticleContact();
	
	// Particles involved in the collision 
	Particle* particle[2];
	
	// Restitution coefficient 
	float restitution;

	// Contact normal, 
	// from the first particle perspective 
	Vector3 contactNormal;

	// Como de solapado esta una particula en otra. p < 0 no se tocan, p = 0 se tocan, p > 0 se solapan
	float penetration;
protected:
	// Resolves this contact for both velocity and interpenetration. 
	void resolve(float t);
	float calculateSeparatingVelocity() const;
private: 
	void resolveVelocity(float t);
	void resolveInterpenetration(float t);
};