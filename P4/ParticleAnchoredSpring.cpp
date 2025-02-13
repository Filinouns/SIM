#include "ParticleAnchoredSpring.h"

ParticleAnchoredSpring::ParticleAnchoredSpring(Vector3* _anchor, float _k, float _restLength) {
	k = _k;
	anchor = _anchor;
	restLength = _restLength;
}

void ParticleAnchoredSpring::updateForce(Particle *particle, double t) {
	Vector3 f = particle->getPos(); 
	f -= *anchor;

	// Length 
	float length = f.normalize();

	// Resulting force 
	f *= -((length - restLength) * k);
	particle->addForce(f);
}