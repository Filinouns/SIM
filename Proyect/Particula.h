#pragma once
#include "core.hpp"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"

using namespace physx;

enum State {
	ON, OFF
};

enum tipoBala {
	Pistola, Artilleria, Fuego, Laser
};

enum ID {
	fire, particle
};

class Particula {
public:
	Particula(RenderItem* particle, PxShape* shape, Vector3 pos_ = {0, 0, 0}, Vector3 acc_ = { 0, 0, 0 }, ID id = particle/*, Vector3 color = {0, 0, 0}*/): pos(pos_), myPos(pos), particle_(particle) {
		particle_->shape = shape;
		maxDistancia = 300;
		myId = id;
		myVel = {0, 0, 0};
		myAcc = acc_;
		force = { 0, 0, 0 };
		damping = 0;
		inverse_mass_ = 0;
		myAge = 0;
		particle_->transform = &myPos;
		particle_->color = Vector4({ 0, 0, 0 , 0});
		setOn();
	};
	virtual ~Particula() {
		particle_->release();
	};

	void clearForce() {
		force.x = 0;
		force.y = 0;
		force.z = 0;
	};

	void addForce(const Vector3& f) {
		force += f;
	};

	void setPos(Vector3 pos) {
		myPos.p = pos;
	}

	int getAge() {
		return myAge;
	}

	bool hasFiniteMass() const {
		return inverse_mass_ >= 0.0f;
	}

	void setMyAge(int age) {
		myAge = age;
	}

	void setVel(Vector3 vel) {
		myVel = vel;
	}

	void setDirVel(Vector3 dir) {
		Vector3 myDir = dir;
		myVel = myDir * getVel().magnitude();
	}

	void setDirAcc(Vector3 acce) {
		Vector3 myAc = acce;
		myAcc = myAc * getAcc().magnitude();
	}

	void setAcc(Vector3 acc) {
		myAcc = acc;
	}

	void setDamp(float damp) {
		damping = damp;
	}

	void setMass(float mass) {
		inverse_mass_ = ((int)1)/mass;
	}

	int getMass() {
		if (inverse_mass_ == 0) { return INT32_MAX;
		}
		else {
			return ((int)1) / inverse_mass_;
		}
	}
	void setColor(Vector4 color_) {
		particle_->color = color_;
	}

	void setTipoBala(tipoBala bala) {
		myBala = bala;
	}

	tipoBala getTipoBala() {
		return myBala;
	}

	PxVec3 getPos() const{
		return myPos.p;
	}

	Vector3 getVel() const{
		return myVel;
	}

	Vector3 getVel(Vector3 *vel) {
		return *vel = Particula::myVel;
	}

	Vector3 getAcc() const{
		return myAcc;
	}

	/*void setShape(PxShape* sh) {
		shape = sh;
	}*/

	PxShape* getShape() {
		return myShape;
	}

	Vector4 getColor() {
		return myColor;
	}

	void integrate(float t) {
		if (this->getState() == OFF) {
			return;
		}
		if (inverse_mass_ <= 0.0f) return;
			// Update position of particle transform
		myPos.p += myVel * t;

		Vector3 tAcceleration = myAcc;

		tAcceleration += force * inverse_mass_;
	

		myVel += tAcceleration * t;

		myVel *= powf(damping, t);

		clearForce();

		if (myId == particle && this->getPos().magnitude() > maxDistancia) {
			setOff();
		}

	}

	 void update(float t) {
		integrate(t);
		myAge += t;
	}

	State getState() {
		return this->myState;
	}

	State getActive() {
		return ON;
	}

	void setOff() {
		myState = State::OFF;
		DeregisterRenderItem(particle_);
		//particle_->release();
	}

	RenderItem* getRender() {
		return particle_;
	}

	void setOn() {
		myState = State::ON;
		particle_->addReference();
		RegisterRenderItem(particle_);
	}

	/*void setState(State newSt) {
		myState = newSt;
		if (this->getState() == OFF) {
			this->release();
			DeregisterRenderItem(this);
		}
		else {
			RegisterRenderItem(this);
		}
	}*/


protected:
	// Position in world space 
	PxTransform myPos;
	Vector3 myVel;
	Vector3 myAcc;
	Vector4 myColor;
	Vector3 pos;
	RenderItem* particle_;

	Vector3 force;

	State myState;

	PxShape* myShape;

	int myAge;

	ID myId;

	float inverse_mass_;
	float damping;
	int maxDistancia;

	tipoBala myBala;

};