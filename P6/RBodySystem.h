#pragma once
#include "PxRigidDynamic.h"
#include "RigidObject.h"
#include "ParticleForceRegistry.h"

using namespace physx;
using namespace std;

class RBodySystem {
private:
	PxScene* gScene = nullptr;
	PxPhysics* gPhysics = nullptr;

	ParticleForceRegistry<RigidObject> *reg;

	vector<ParticleForceGenerator*> forces;

	const int MAX_RIGIDS = 30;
	int numRigidBodies;

	static float randomfl(float min, float max) {
		float r = (float)rand() / (float)RAND_MAX;
		return min + r * (max - min);
	};

	vector<RigidObject*> bodies;

	float secondsPassed = 0;
	float secondsToDelay = 0.5;

	bool activo;

public:
	RBodySystem(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_) {
		activo = false;
		reg = new ParticleForceRegistry<RigidObject>();
		gScene = gScene_;
		gPhysics = gPhysics_;
		numRigidBodies = 0;
	};
	virtual ~RBodySystem() {
		delete reg;
		reg = nullptr;

		for (auto b : bodies) {
			//b->getRender()->release();
			delete b;
			b = nullptr;
		}
	};

	void update(float t) {
		if (activo) {
			secondsPassed += t;
			if (secondsPassed > secondsToDelay) {
				if (rand() % 2 == 0)
					creaRigidDynamic();
				else {
					creaRigidStatic();
				}
				secondsPassed = 0;
			}
		}
		for (auto b : bodies) {
			if (b->getCanDisappear()) {
				if (!b->update(t)) {
					//b->getRender()->activo = false;
					if (!b->getProcesado()) {
						numRigidBodies = numRigidBodies - 1;
						b->setProcesadoTrue();
					}
				}
			}
		}
		//cout << numRigidBodies << endl;
	};

	RigidObject* creaRigidDynamic(Vector3 pos = { 0, 0, 0 }) {
		if (numRigidBodies < MAX_RIGIDS) {
			Vector3 dir = { randomfl(1, 70), randomfl(1, 200), randomfl(1, 70) };
			dir.normalize();
			RigidObject* newBodyDy = new RigidObject(gScene, gPhysics, CreateShape(*new PxBoxGeometry(2, 2, 2)), pos, -10.0f, dir, true);
			newBodyDy->createBodyDynamic();
			addGenToReg(newBodyDy);
			numRigidBodies++;
			bodies.push_back(newBodyDy);
			return newBodyDy;
		}
	}

	RigidObject* creaRigidStatic(Vector3 pos = { randomfl(-50, 50), randomfl(0, 10), randomfl(-50, 50) }) {
		if (numRigidBodies < MAX_RIGIDS) {
			RigidObject* newBodySt = new RigidObject(gScene, gPhysics, CreateShape(*new PxSphereGeometry(2.0f)), pos, 0, { 0, 0, 0 }, false);
			newBodySt->createBodyStatic();
			addGenToReg(newBodySt);
			numRigidBodies++;
			bodies.push_back(newBodySt);
			return newBodySt;
		}
	}

	RigidObject* creaSuelo(Vector3 pos = { 0, -20, 0 }) {
		RigidObject* newBodySuelo = new RigidObject(gScene, gPhysics, CreateShape(*new PxBoxGeometry(100, 0.5, 100)), pos, 0, { 0, 0, 0 }, false);
		newBodySuelo->createBodyStatic();
		return newBodySuelo;
	}

	void addForceToGens(ParticleForceGenerator* newForce) {
		forces.push_back(newForce);
	}

	void addGenToReg(RigidObject* p) {
		for (int i = 0; i < forces.size(); i++) {
			reg->add(p, forces[i]);
		}
	}

	ParticleForceRegistry<RigidObject>* getReg(ParticleForceRegistry<RigidObject>* reg_) {
		return reg = reg_;
	};

	void setActivo() {
		activo = !activo;
	}

	bool getActivo() {
		return activo;
	}
};

