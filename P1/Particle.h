#pragma once

#include "core.hpp"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"

using namespace physx;

enum State {
	ON, OFF
};

class Particle {
public:
	Particle(PxShape* shape, Vector4 color, float mass);
	Particle() {};
	virtual ~Particle();

	//---Funciones de atributos---
	void setPos(Vector3 pos) { pos_.p = pos; }
	void setVel(Vector3 vel) { vel_ = vel; }
	Vector3 getVel() { return vel_; }
	void setAcc(Vector3 acc) { acc_ = acc; }
	void setDump(float damp) { damping_ = damp; }
	void setState(State s) { this->state_ = s; }
	State getState() { return this->state_; }
	int getAge() { return age_; }

	void init(Vector3 p, Vector3 v, Vector3 acc, float d);
	void integrate(float t);
	void update(float t);

private:
	PxTransform pos_;	// Posicion
	Vector3 vel_;	// Velocidad
	Vector3 dir_;	// Direccion
	Vector3 acc_;	// Aceleracion
	Vector4 color_;	// Color

	RenderItem *renderItem_;

	float age_;
	State state_;

	float inverse_mass;
	float damping_;	// Frenado (Rozamiento)
};