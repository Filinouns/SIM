#pragma once

#include <list>
#include "core.hpp"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"
#include "Particle.h"
#include "FireWork.h"

#include <random>

using namespace std;

enum SystemType {
	Fountain, Fireworks 
};

class ParticleSystem {
public:
	ParticleSystem(PxGeometry *g, Vector4 color, Vector3 p, SystemType t);
	virtual ~ParticleSystem();

	void update(float t);
	void createParticle();
	void createFirework();

	Vector4 selectRndColor();

private:
	Vector4 color1 = { 1, 0, 0, 0 };
	Vector4 color2 = { 0, 1, 0, 0 };

	// Particulas
	list<Particle*> particles_;

	Vector4 color_;
	float countdown_;
	float iniCount_;

	//Sistema
	PxTransform pos_;
	SystemType type_;
};