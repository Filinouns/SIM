#pragma once
#include "ParticleForceGenerator.h"
#include <iostream>

class ParticleAnchoredSpring : public ParticleForceGenerator {
	// Position of the other end of the spring 
	Vector3 *anchor;
	// Elasticity constant 
	float k;

	float restLength;
public:
	ParticleAnchoredSpring(Vector3 *_anchor, float _k, float _restLength);
	virtual void updateForce(Particle* particle, float t);
	//Vector3 getAnchor() { return *anchor; }
	void setAnchor(PxVec3 *a) { anchor = a; }
	void addK() { k++; std::cout << k << "\n"; }
	void subK() { k--; std::cout << k << "\n"; }
};