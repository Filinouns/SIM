#pragma once
#include "ParticleForceGenerator.h"
class ParticleDrag : public ParticleForceGenerator {

	//Coeficiente para la velocidad
	float k1;

	//Coeficiente para la velocidad al cuadrado
	float k2;

public:
	ParticleDrag(float _k1, float _k2) : k1(_k1), k2(_k2) {}
	virtual ~ParticleDrag();

	virtual void updateForce(Particle* p, float t);
};

