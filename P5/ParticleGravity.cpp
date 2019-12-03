#include "ParticleGravity.h"

ParticleGravity::~ParticleGravity() {}

void ParticleGravity::updateForce(Particle * p, double t) {
	if (!p->hasInfiniteMass()) return;

	p->addForce(g * p->getMass());
}
