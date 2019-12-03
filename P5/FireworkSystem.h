#pragma once
#include "ParticleSystem.h"
#include "FireWork.h"

class FireworkSystem : public ParticleSystem {
public:
	FireworkSystem(Vector3 p);
	virtual ~FireworkSystem();

	virtual void update(float t);
	virtual void particleGenerator(float t);

	void createParticle(int t);
	void createFirework(PxShape* s, Vector3 vel);
	void createSon(Vector3 p);

private:
	FireWork* firework = NULL;
};