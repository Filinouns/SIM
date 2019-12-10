#include "ParticleGravity.h"

void ParticleGravity::updateForce(Particula* particle, float t) {
	if (!particle->hasFiniteMass()) {
		return;
	}
	particle->addForce(g*particle->getMass());
}

void ParticleGravity::updateForce(RigidObject* particle, float t) {
	if(particle->getCanMove())
		particle->getBody()->addForce(g*particle->getBody()->getMass());
}