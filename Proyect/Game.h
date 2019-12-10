#pragma once
#include "RigidBody.h"
//#include "Character.h"
#include "Explosion.h"
#include "Wind.h"
#include "Particula.h"
#include "FireworkManager.h"
//#include "HandleManager.h"
#include "Sistema.h"
#include "ParticleBuoyancy.h"

class Game {
public:
	Game(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_);
	virtual ~Game();

	void init();
	void update(double t);
	void keyPress(unsigned char key);

private:
	RigidBody* rb;
	RigidBody* bullets;
	RigidBody* targets;

	//Character* player;
	Sistema* lluvia;
	//ParticleBuoyancy* pBuoyancy;
	FireworkManager* fManager;
	Explosion* explosion;
	Wind* wind;

	ParticleForceRegistry<RigidObject>* pFRegRigids;
	ParticleForceRegistry<Particula>* pFRegParticulas;

	physx::PxScene* gScene;
	physx::PxPhysics* gPhysics;

	void checkColissions();
};