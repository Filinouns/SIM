#include "Particle.h"

Particle::Particle(float m, RenderItem* rit) {
	damping_ = 0.2;

	inverse_mass = m;
	
	rend_it = rit;
}

Particle::~Particle() {}

void Particle::integrate(float t) {
	if (inverse_mass <= 0.0f) return;

	// Update pos
	pos_ += vel_ * t;

	// Update linear vel
	vel_ += acc_ * t;

	// Impose drag (damping)
	vel_ *= powf(damping_, t);
}