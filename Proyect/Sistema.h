#pragma once
#include "Particula.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include "ParticleForceRegistry.h"

class Sistema
{

private:

	ParticleForceRegistry<Particula>* reg;

	std::vector<ParticleForceGenerator*> forces;

	static float randomfl(float min, float max)
	{
		float r = (float)rand() / (float)RAND_MAX;
		return min + r * (max - min);
	};
	tipoBala myBala;

	std::vector<Particula*> particulas;
	
	float startTime = 0.5;
	float secondsPassed = 0;
	float secondsToDelay = 0.01;

public:
	Sistema() {
	};

	void update(float t) {
		tipoBala newB = tipoBala(rand() % 1);
		/*if (numParticles < MAX_PARTICLES) {*/
		creaParticula(t);
		for (auto part : particulas) {
			part->update(t);
		}
	};

	Particula* creaParticula(float t) {
		secondsPassed += t;
		if (secondsPassed > secondsToDelay) {
			RenderItem* rItem = new RenderItem();
			Particula* newParticula = new Particula(rItem, CreateShape(*new PxSphereGeometry(1.0f)));
			newParticula->setPos({ -20, 10, -20 });
			Vector3 dir = { randomfl(1, 70), randomfl(1, 200), randomfl(1, 70) };
			dir.normalize();
			newParticula->setAcc(dir*100);
			newParticula->setVel(dir*100);
			newParticula->setDamp(0.99f);
			//newParticula->setDirVel({ -randomfl(1, 10), 0, -randomfl(1, 10) });
			newParticula->setMass(1.0f);
			newParticula->setColor({ 0.3, 1.0, 0.2, 1 });
			newParticula->setOn();
			addGenToReg(newParticula);
			particulas.push_back(newParticula);
			secondsPassed = 0;
			return newParticula;
		}
	}

	void addForceToGens(ParticleForceGenerator* newForce) {
		forces.push_back(newForce);
	}

	void addGenToReg(Particula* p) {
		for (int i = 0; i < forces.size(); i++) {
			reg->add(p, forces[i]);
		}
	}

	ParticleForceRegistry<Particula>* getReg(ParticleForceRegistry<Particula>* reg_) {
		return reg = reg_;
	};

	virtual ~Sistema() {
		for (auto p : particulas)
		{
			delete p;
			p = nullptr;
		}
		for (auto f : forces) {
			f = nullptr;
			delete f;
		}
		reg = nullptr;
		delete reg;
	};

};


