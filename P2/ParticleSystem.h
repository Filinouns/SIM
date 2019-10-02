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

enum ParticleType {
	Esfera, Capsula, SuperCapsula, SuperEsfera
};

class ParticleSystem {
public:
	ParticleSystem(Vector3 p, SystemType t);
	virtual ~ParticleSystem();

	void update(float t);
	void createParticle();
	void createFirework();

	void createParticle(int t);

	Vector4 rnd_Color();

	int MAX_PARTICLES = 20;

private:
	Vector4 color1	=	{ 1, 0, 0, 0 };
	Vector4 color2	=	{ 0, 1, 0, 0 };
	Vector4 red		=	{ 1, 0, 0, 1 };
	Vector4 magenta =	{ 1, 0, 1, 1 };
	Vector4 blue	=	{ 0, 0, 1, 1 };
	Vector4 yellow	=	{ 1, 0.92, 0.016, 1 };

	// Particulas
	list<Particle*> particles_;
	Particle* particle = NULL;

	Vector3 gravity = { 0, -10, 0 };

	Vector4 color_;
	float countdown_;
	float iniCount_;

	//Sistema
	PxTransform pos_;
	SystemType type_;

	int n_Particles;
};