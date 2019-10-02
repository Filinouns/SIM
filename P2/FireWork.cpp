#include "FireWork.h"

FireWork::FireWork(PxShape* s, Vector4 color, float m) : Particle(s, color, m) {
	generations_ = (rand() % 3) + 2;
	exploted_ = false;

	//pos_.rotate({ 90, 90, 90 });
	//Probar aqui para la direccion tocar el pxTransform
	//this->getTrans().rotate();
}

FireWork::~FireWork() {
	for (auto p : sub_particles) {
		delete p;
	}
	Particle::~Particle();
}

void FireWork::update(float t) {
	switch (state_) {
	case State::ON:
		age_ += t;
		integrate(t);
		if (age_ >= max_age) {
			state_ = State::INVISIBLE;
		}
		break;
	case State::OFF:

		break;
	case State::INVISIBLE:
		if (!exploted_) {
			explote();
			this->renderItem_->release();
		}
		else {
			if (sub_particles.size() != 0) {
				for (auto p : sub_particles) p->update(t);
			}
			Particle *p = *sub_particles.begin();
			if (p->getState() == State::OFF) state_ = State::OFF;
		}
		break;
	default:
		break;
	}
}

void FireWork::explote() {
	exploted_ = true;
	//CreateShape(PxCapsuleGeometry(1, 2));
	//PxGeometry* s = new PxSphereGeometry(2);
	//PxShape* shape = CreateShape(*s);
	for (int i = 0; i < generations_; i++) {
		Particle* p = new Particle(CreateShape(PxCapsuleGeometry(1, 2)), randColor(), 2.5f);
		p->init(
			this->getPos(),
			{ static_cast<float>(rand() % 20),
			15,
			static_cast<float>(rand() % 5) },
			{ 0, -10, 0 },
			0.99);

		p->setMaxAge(5.0f);
		p->setState(State::ON);

		sub_particles.push_back(p);
	}

	//shape->release();
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