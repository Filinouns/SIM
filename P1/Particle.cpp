#include "Particle.h"

Particle::Particle(PxShape* shape, Vector4 color, float mass) {
	damping_ = 0.2;

	this->state_ = State::OFF;
	pos_.p = { 0, 0, 0 };
	pos_.q = { 0, 0, 0, 0 };
	vel_ = { 0, 0, 0 };
	acc_ = { 0, 0, 0 };
	color_ = color;
	inverse_mass = 1/mass;
	damping_ = 0;

	age_ = 0;

	createItem(shape);
}

Particle::~Particle() {}

void Particle::update(float t) {
	if (this->state_ == State::ON) {
		age_++;
		integrate(t);
	}
}

void Particle::integrate(float t) {
	if (inverse_mass <= 0.0f) return;

	// Update pos
	pos_.p += vel_ * t;

	// Update linear vel
	vel_ += acc_ * t;

	// Impose drag (damping)
	vel_ *= powf(damping_, t);
}