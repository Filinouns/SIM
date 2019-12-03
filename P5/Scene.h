#pragma once


//Sistemas
#include "ParticleSystem.h"
#include "FireworkSystem.h"

//Fuerzas
#include "ParticleGravity.h"
#include "Wind.h"
#include "Explosion.h"

//Muelles
#include "ParticleAnchoredSpring.h"	// Muelle anclado a un punto
#include "ParticleBungee.h"			// Goma elastica
#include "ParticleBuoyancy.h"		// Flotacion
#include "ParticleSpring.h"			// Muelle entre 2 objetos movibles

//Constraints
#include "ParticleContact.h"
#include "ParticleContactManager.h"
#include "ParticleCable.h"
#include "ParticleRod.h"

#include <PxPhysicsAPI.h>

class Scene {
public:
	Scene();
	virtual ~Scene();

	void Init();
	void update(double t);
	void keyPress(unsigned char key, const PxTransform& camera);

private:
	void generateParticleSystems();
	void generateSprings();
	void generateConstraints();
};