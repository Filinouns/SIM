#pragma once

#include "core.hpp"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"

using namespace physx;

enum State {
	ON, OFF, INVISIBLE,
};

class Particle {
public:
	Particle(PxShape* shape, Vector4 color, float mass);
	Particle() {};
	virtual ~Particle();

	//---Funciones de atributos---
	void setPos(Vector3 pos) { pos_.p = pos; }
	Vector3 getPos() { return pos_.p; }
	void setTrans(PxTransform p) { pos_ = p; }
	PxTransform getTrans() { return pos_; }
	void setVel(Vector3 vel) { vel_ = vel; }
	Vector3 getVel() { return vel_; }
	void setAcc(Vector3 acc) { acc_ = acc; }
	void setDump(float damp) { damping_ = damp; }
	void setState(State s) { this->state_ = s; }
	State getState() { return this->state_; }
	int getAge() { return age_; }
	void setMaxAge(float o) { max_age = o; }

	// Funciones generales
	virtual void init(Vector3 p, Vector3 v, Vector3 acc, float d);
	virtual void integrate(float t);
	virtual void update(float t);

	//Variables publicas
	RenderItem *renderItem_;

protected:
	PxTransform pos_;	// Posicion
	Vector3 vel_;	// Velocidad
	Vector3 dir_;	// Direccion
	Vector3 acc_;	// Aceleracion
	Vector4 color_;	// Color

	float max_age;
	float age_;
	State state_;

	float inverse_mass;
	float damping_;	// Frenado (Rozamiento)
};