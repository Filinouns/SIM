#include "ParticleSpring.h"

ParticleSpring::ParticleSpring(Particle *_other, float _k, float _restLength) {
	other = _other;
	k = _k;
	restLength = _restLength;
}

void ParticleSpring::updateForce(Particle *particle, double t) {
	// Calculate distance vector 
	Vector3 f = particle->getPos(); 
	f -= other->getPos();

	// Length 
	float length = f.normalize();

	// Resulting force 
	float deltaL = (length - restLength); 
	float forceMagnitude = -k * deltaL; 
	f *= forceMagnitude; 
	particle->addForce(f);
}