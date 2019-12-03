#include "ParticleDrag.h"

ParticleDrag::~ParticleDrag() {}

void ParticleDrag::updateForce(Particle * p, float t) {
	Vector3 f = p->getVel();

	// Coeficiente de drag
	float dragCoeff = f.normalize();
	dragCoeff = k1 * dragCoeff + k2 * dragCoeff * dragCoeff;

	// Fuerza final
	f *= -dragCoeff;
	p->addForce(f);
}
