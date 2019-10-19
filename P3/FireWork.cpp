#include "FireWork.h"

FireWork::FireWork(PxShape* s, Vector4 color, float m) : Particle(s, color, m) {
	generations_ = (rand() % 3) + 2;
	exploted_ = false;
	detected_ = false;

	//pos_.rotate({ 90, 90, 90 });
	//Probar aqui para la direccion tocar el pxTransform
	//this->getTrans().rotate();
}

FireWork::~FireWork() {
	Particle::~Particle();
}

void FireWork::update(float t) {
	switch (state_) {
	case State::ON:
		age_ += t;
		integrate(t);
		if (age_ >= max_age) state_ = State::INVISIBLE;
		break;
	case State::OFF:
		break;
	case State::INVISIBLE:
		if (!exploted_) {
			//explote();
			exploted_ = true;
			this->renderItem_->release();
		}
		if(detected_) {
			state_ = State::OFF;
		}
		break;
	default:
		break;
	}
}