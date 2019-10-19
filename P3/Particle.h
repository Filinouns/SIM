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

	//Variables publicas
	RenderItem *renderItem_;

	// Funciones generales
	virtual void init(Vector3 p, Vector3 v, float d);
	virtual void integrate(float t);
	virtual void update(float t);

	//---Funciones de atributos---
	//Posicion
	void setPos(Vector3 pos) { pos_.p = pos; }
	Vector3 getPos() { return pos_.p; }
	//Transform
	void setTrans(PxTransform p) { pos_ = p; }
	PxTransform getTrans() { return pos_; }
	//Velocidad
	void setVel(Vector3 vel) { vel_ = vel; }
	Vector3 getVel() { return vel_; }
	//Aceleracion
	void setAcc(Vector3 acc) { acc_ = acc; }
	//Rozamiento
	void setDump(float damp) { damping_ = damp; }
	//Estado
	void setState(State s) { this->state_ = s; }
	State getState() { return this->state_; }
	//Edad
	int getAge() { return age_; }
	void setMaxAge(float o) { max_age = o; }
	//Masa
	bool hasInfiniteMass() const { return inverse_mass >= 0.0f; }
	int getMass() {
		if (inverse_mass == 0) return INT32_MAX;
		else return ((int)1) / inverse_mass;
	}
	//Fuerzas
	void clearForce(); //Limpia la fuerza acumulada
	void addForce(const Vector3& f); //Añade fuerza para que se aplique unicamente en la siguiente iteracion

protected:
	PxTransform pos_;	// Posicion
	Vector3 vel_;	// Velocidad
	Vector3 dir_;	// Direccion
	Vector3 acc_;	// Aceleracion
	Vector4 color_;	// Color
	Vector3 force_;

	float max_age;
	float age_;
	State state_;

	float inverse_mass;
	float damping_;	// Frenado (Rozamiento)
};