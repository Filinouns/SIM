#include "ParticleGravity.h"


ParticleGravity::~ParticleGravity() {}

void ParticleGravity::updateForce(Particle * p, float t) {
	if (!p->hasInfiniteMass()) return;

	p->addForce(g * p->getMass());
}
