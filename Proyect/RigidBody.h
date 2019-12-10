#pragma once
#include "PxRigidDynamic.h"
#include "RigidObject.h"
#include "ParticleForceRegistry.h"

class RigidBody {
private:
	physx::PxScene* gScene = nullptr;
	physx::PxPhysics* gPhysics = nullptr;

	ParticleForceRegistry<RigidObject>* reg;

	std::vector<ParticleForceGenerator*> forces;

	const int MAX_RIGIDS = 60;
	int numRigidBodies;

	static float randomfl(float min, float max)
	{
		float r = (float)rand() / (float)RAND_MAX;
		return min + r * (max - min);
	};

	std::vector<RigidObject*> bodies;

	float secondsPassed = 0;
	float secondsToDelay;
	bool canGenerate;

	RigidObject* newBodyDy;
	RigidObject* newBodySt;

	Vector3 pos;

public:
	RigidBody(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, bool canGenerate_ = false, Vector3 pos_ = { 0, 0, 0 }, float secondsDelay_ = randomfl(0.1f, 0.5f)) {
		activo = true;
		reg = new ParticleForceRegistry<RigidObject>();
		gScene = gScene_;
		gPhysics = gPhysics_;
		numRigidBodies = 0;
		pos = pos_;
		secondsToDelay = secondsDelay_;
		canGenerate = canGenerate_;
	};
	virtual ~RigidBody() {
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
			if (canGenerate) {
				secondsPassed += t;
				if (secondsPassed > secondsToDelay) {
					creaObjetivo();
					secondsPassed = 0;
					secondsToDelay = randomfl(0.4f, 1.2f);
				}
			}
			int it = 0;
			for (auto b : bodies) {
				if (b->getCanDisappear()) {
					b->getRender()->activo = false;
					//if (b->getBody() != nullptr) b->getBody()->release();
					//else if (b->getbodySt() != nullptr) b->getbodySt()->release();
					delete b;
					b = nullptr;
					bodies.erase(bodies.begin() + it);
				}
				it++;
			}
		}
	};
	
	RigidObject* creaRigidDynamic(Vector3 pos = { 0, 0, 0 }, PxVec4 color = { randomfl(0, 1), randomfl(0, 1), randomfl(0, 0.5f), 1 }, PxShape* myShape = CreateShape(*new PxCapsuleGeometry(2.0f, 2.0f))) {
		Vector3 dir = { randomfl(1, 1), randomfl(1, 200), randomfl(1,1) };
		dir.normalize();
		newBodyDy = new RigidObject(gScene, gPhysics, myShape, pos, -10.0f, dir, true);
		newBodyDy->createBodyDynamic();
		addGenToReg(newBodyDy);
		numRigidBodies++;
		bodies.push_back(newBodyDy);
		return newBodyDy;
	}
	
	RigidObject* creaRigidStatic(Vector3 pos = { randomfl(-50, 50), randomfl(0, 10), randomfl(-50, 50) }) {
		RigidObject* obj = new RigidObject(gScene, gPhysics, CreateShape(*new PxBoxGeometry(2.0f, 2.0f, 2.0f)), pos, 0, { 0, 0, 0 }, false);
		obj->createBodyStatic();
		return obj;
	}
	
	RigidObject* creaBala(Vector3 pos, Vector3 dir) {
		if (numRigidBodies < MAX_RIGIDS) {
			dir.normalize();
			RigidObject* newBala = new RigidObject(gScene, gPhysics, CreateShape(*new PxSphereGeometry(2.0f)), pos, 100.0f, dir, true);
			newBala->setCanDisappear(false);
			newBala->createBodyDynamic();
			addGenToReg(newBala);
			numRigidBodies++;
			bodies.push_back(newBala);
			return newBala;
		}
	}

	RigidObject* creaObjetivo() {
		if (numRigidBodies < MAX_RIGIDS) {
			Vector3 pos = { randomfl(-50, 50), randomfl(0, 10), randomfl(-50, 50) };
			RigidObject* newBodySt = new RigidObject(gScene, gPhysics, CreateShape(*new PxSphereGeometry(2.0f)), pos, 0, { 0, 0, 0 }, false);
			newBodySt->setCanDisappear(false);
			newBodySt->createBodyStatic();
			bodies.push_back(newBodySt);
			return newBodySt;
		}
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

	std::vector<RigidObject*> getBodies() {
		return bodies;
	}

	void setActivo() {
		activo = !activo;
	}


	bool getActivo() {
		return activo;
	}

	virtual void keyPressed(unsigned char key, const PxTransform& camera) {
		switch (toupper(key)) {
		case 'X':
		{
			creaRigidDynamic();
			break;
		}
		case 'C':
		{
			creaRigidStatic();
			break;
		}
		case 'V':
		{
			/*creaSuelo();*/
			break;
		}
		case 'B':
		{
			setActivo();
			break;
		}
		}
	}
	bool activo;

	void destroyBodi(RigidObject* b) {
		for (auto bodi : bodies) {
			if (bodi) {
				if (bodi == b) {
					bodi->setCanDisappear(true);
					b->setCanDisappear(true);
				}
			}
		}
	}
};