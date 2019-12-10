#pragma once
#include "core.hpp"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"
class RigidObject
{
private:
	PxTransform myPos;
	PxShape* myShape;
	float myVel;
	Vector3 myDir;

	PxRigidDynamic* body = nullptr;
	PxRigidStatic* bodySt = nullptr;
	RenderItem* render = nullptr;

	bool canMove;
	bool canDisappear;
	bool procesado;

	float myAge;
	float maxAge; 
	
	physx::PxScene* gScene = nullptr;
	physx::PxPhysics* gPhysics = nullptr;

public:
	RigidObject(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_, PxShape* shape, Vector3 pos = {0, 0, 0}, float vel = 0, Vector3 dir = { 0, 0, 0 }, bool canMove_ = true, bool canDisappear_ = true, float maxAge_ = 10) : myPos(pos) {
		gScene = gScene_;
		gPhysics = gPhysics_;
		myShape = shape;
		myVel = vel;
		myDir = dir;
		canMove = canMove_;
		myAge = 0;
		maxAge = maxAge_;
		canDisappear = canDisappear_;
		procesado = false;
	};
	virtual ~RigidObject() {
		//myShape->release();
	};

	bool update(float t) {
		myAge += t;
		return (myAge < maxAge);
	}

	PxRigidDynamic* createBodyDynamic() {
		body = gPhysics->createRigidDynamic(myPos);
		body->attachShape(*myShape);
		render = new RenderItem(myShape, body, { 0, 1, 0, 1 });
		PxRigidBodyExt::updateMassAndInertia(*body, 1);
		body->setLinearVelocity(myDir * myVel);
		gScene->addActor(*body);
		myShape->release();
		return body;
	}

	PxRigidStatic* createBodyStatic() {
		bodySt = gPhysics->createRigidStatic(myPos);
		bodySt->attachShape(*myShape);
		render = new RenderItem(myShape, bodySt, { 1,0 , 0, 1 });
		gScene->addActor(*bodySt);
		myShape->release();
		return bodySt;
	}

	PxRigidDynamic* getBody() { 
		if (body != nullptr) return body;
	}

	PxRigidStatic* getbodySt() { if (bodySt != nullptr) return bodySt; }

	RenderItem* getRender() {
		return render;
	}

	bool getCanMove() {
		return canMove;
	}

	bool getCanDisappear() {
		return canDisappear;
	}

	void setCanDisappear(bool d) {
		canDisappear = d;
	}

	bool getProcesado() {
		return procesado;
	}
	void setProcesadoTrue() {
		procesado = true;
	}
};