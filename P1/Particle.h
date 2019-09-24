#pragma once

#include "core.hpp"
#include "RenderUtils.hpp"

class Particle {
public:
	Particle(float mass, RenderItem* rit);
	virtual ~Particle();

	void integrate(float t);
	RenderItem* getRend() { return rend_it; }

private:
	Vector3 pos_;	// Posicion
	Vector3 vel_;	// Velocidad
	Vector3 dir_;	// Direccion

	Vector3 acc_;	// Aceleracion
	
	RenderItem *rend_it;

	//PxTransform transform_;

	float inverse_mass;
	float damping_;	// Frenado (Rozamiento)
};

