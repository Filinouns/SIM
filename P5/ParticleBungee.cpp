#include "ParticleBungee.h"

ParticleBungee::ParticleBungee(Particle * _other, float _k, float _restLength) {
	other = _other;
	k = _k;
	restLength = _restLength;
}

void ParticleBungee::updateForce(Particle * particle, float t) {
	Vector3 f = particle->getPos(); 
	f -= other->getPos();
	// Length 
	float length = f.normalize(); 
	length = (length - restLength); 
	if (length <= 0.0f) return;
	// Resulting force 
	f *= -(length * k); 
	particle->addForce(f);
}