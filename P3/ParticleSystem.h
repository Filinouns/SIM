#pragma once

#include <list>
#include "core.hpp"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"
#include "Particle.h"

#include "ParticleForceGenerator.h"
#include "ParticleForceRegistry.h"

#include <random>

using namespace std;

enum ParticleType {
	Esfera, Capsula, SuperCapsula, SuperEsfera
};

class ParticleSystem {
public:
	ParticleSystem(Vector3 p);
	virtual ~ParticleSystem();

	//Update
	virtual void update(float t);
	//Aux Update
	virtual void deleteParticles(float t);
	virtual void particleGenerator(float t);


	// Creacion de particulas
	void createParticle();
	//void createFirework(PxShape* s, Vector3 vel);
	//void createParticle(int t);
	Vector4 rnd_Color();

	//Fuerzas
	void registerForce(ParticleForceGenerator* fg);
	void addForcesToPart(Particle* p);
	void setReg(ParticleForceRegistry* r) { regist = r; }

	int MAX_PARTICLES = 20;

protected:
	// Colores
	Vector4 color1	=	{ 1, 0, 0, 0 };
	Vector4 color2	=	{ 0, 1, 0, 0 };
	Vector4 red		=	{ 1, 0, 0, 1 };
	Vector4 magenta =	{ 1, 0, 1, 1 };
	Vector4 blue	=	{ 0, 0, 1, 1 };
	Vector4 yellow	=	{ 1, 0.92, 0.016, 1 };

	// Particulas
	list<Particle*> particles_;
	Particle* particle = NULL;

	//Fuerzas
	ParticleForceRegistry* regist;
	std::vector<ParticleForceGenerator*> forces_;

	Vector4 color_;
	float countdown_;
	float iniCount_;

	//Sistema
	PxTransform pos_;
	//SystemType type_;

	int n_Particles;
};