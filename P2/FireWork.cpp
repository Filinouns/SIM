#include "FireWork.h"

FireWork::FireWork(PxShape* s, Vector4 color, float m) : Particle(s, color, m) {
	generations_ = rand() % 5 + 2;
	exploted_ = false;
}

FireWork::~FireWork() {
	for (auto p : sub_particles) {
		delete p;
	}
	Particle::~Particle();
}

void FireWork::update(float t) {
	if (this->getState() == State::OFF && !exploted_) {
		explote();
	}

	if (sub_particles.size() != 0) {
		for (auto p : sub_particles) p->update(t);
	}

	Particle::update(t);
}

void FireWork::explote() {
	exploted_ = true;
	PxGeometry* s = new PxSphereGeometry(2);
	PxShape* shape = CreateShape(*s);
	for (int i = 0; i < generations_; i++) {
		Particle* p = new Particle(shape, randColor(), 2.5f);
		p->init(
			this->getPos(),
			{ static_cast<float>(rand() % 20),
			15,
			static_cast<float>(rand() % 5) },
			{ 0, -10, 0 },
			0.99);

		p->setMaxAge(2.0f);

		sub_particles.push_back(p);
	}

	shape->release();
}

Vector4 FireWork::randColor() {
	Vector4 c;
	switch (rand() % 4) {
	case 0:
		c = red;
		break;
	case 1:
		c = magenta;
		break;
	case 2:
		c = blue;
		break;
	case 3:
		c = yellow;
		break;
	default:
		c = red;
		break;
	}
	return c;
}