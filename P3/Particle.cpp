#include "Particle.h"

Particle::Particle(PxShape* shape, Vector4 color, float mass) : RenderItem(shape, &pos_, color){
	this->state_ = State::OFF;
	pos_.p = { 0, 0, 0 };
	pos_.q = { 0, 0, 0, 0 };
	vel_ = { 0, 0, 0 };
	acc_ = { 0, 0, 0 };
	force_ = { 0, 0, 0 };

	color_ = color;
	inverse_mass = 1/mass;
	damping_ = 0;

	age_ = 0;
	max_age = 20.0f;
}

void Particle::init(Vector3 p, Vector3 v, float d) {
	this->setPos(p);
	this->setVel(v);
	this->setDump(d);
	this->setState(State::ON);
}

void Particle::update(float t) {
	age_ += t;
	integrate(t);
}

void Particle::integrate(float t) {
	if (inverse_mass <= 0.0f) return;
	if (this->getState() == OFF) return;

	// Update pos
	pos_.p += vel_ * t;

	Vector3 totalAcc = acc_;
	totalAcc += force_ * inverse_mass;

	// Update linear vel
	vel_ += totalAcc * t;

	// Impose drag (damping)
	vel_ *= powf(damping_, t);

	clearForce();

	if (age_ >= max_age) {
		DeregisterRenderItem(this);
		this->state_ = State::OFF;
	}
}

void Particle::clearForce() {
	force_.x = 0;
	force_.y = 0;
	force_.z = 0;
}

void Particle::addForce(const Vector3& f) {
	force_ += f;
}