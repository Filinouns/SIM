#pragma once
#include "Particula.h"

enum Bala {
	rojo, azul, verde, morado
};

class Firework :
	public Particula, public RenderItem
{
public:
	Firework(RenderItem* render, PxShape* shape = CreateShape(*new PxSphereGeometry(1.5)), Vector3 pos = { 0, 0, 0 }, Vector3 acc = {0, 0, 0}) : Particula(render ,shape, pos, acc, fire) {
		particle_->addReference();
	}
	virtual ~Firework() {}

	bool update(float t) {
		Particula::update(t);
		age -= t;
		return (age < 0);
	}

	Bala getType() const{
		return type;
	}

	void setFireOff() {

	}
	void setType(Bala ty) {
		type = ty;
		switch (type)
		{
		case rojo:
			particle_->color = { 1, 0, 0, 1 };
			break;
		case azul:
			particle_->color = { 0, 0, 1,1 };
			break;
		case verde:
			particle_->color = { 0, 1, 0, 1};
			break;
		case morado:
			particle_->color = { 1, 0, 1, 1 };
			break;
		default:
			break;
		}
	}

	float getAge() const {
		return age;
	}
	void setAge(float ag) {
		age = ag;
	}
private:
	// Type of payload it has (what it will generate)
	Bala type;

	RenderItem* r;

	// Time left for the firework to detonate
	// When age reaches zero, the particle disappears and delivers the payload
	float age;
};

