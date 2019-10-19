#include "Particle.h"

Particle::Particle(PxShape* shape, Vector4 color, float mass) {
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

	renderItem_ = new RenderItem(shape, &pos_, color_);
}

void Particle::init(Vector3 p, Vector3 v, float d) {
	this->setPos(p);
	this->setVel(v);
	//this->setAcc(acc);
	this->setDump(d);
	this->setState(State::ON);
}

Particle::~Particle() {
	if (this->renderItem_) { this->renderItem_->release(); }
}

void Particle::update(float t) {
	switch (this->state_) {
	case State::ON:
		age_ += t;
		integrate(t);
		if (age_ >= max_age) {
			this->state_ = State::OFF;
		}
		break;
	case State::OFF:
		if (this->renderItem_) { this->renderItem_->release(); }
		break;
	case State::INVISIBLE:
		if (this->renderItem_) { this->renderItem_->release(); }
		break;
	default:
		break;
	}
}

void Particle::integrate(float t) {
	if (inverse_mass <= 0.0f) return;

	// Update pos
	pos_.p += vel_ * t;

	Vector3 totalAcc = acc_;
	totalAcc += force_ * inverse_mass;

	// Update linear vel
	vel_ += totalAcc * t;

	// Impose drag (damping)
	vel_ *= powf(damping_, t);

	clearForce();
}

void Particle::clearForce() {
	force_.x = 0;
	force_.y = 0;
	force_.z = 0;
}

void Particle::addForce(const Vector3& f) {
	force_ += f;
}