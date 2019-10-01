#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <list>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include "Particle.h"
#include "ParticleSystem.h"

using namespace physx;
using namespace std;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

PxGeometry*				Sphere_		= NULL;	//Esfera
ParticleSystem*		ParticleSystem_ = NULL; //Sistema de particulas
Particle*			   	 myParticle = NULL; //Particula
		
Vector4 color = { 0.1, 0.3, 1, 0};
list<Particle*> particles_;
list<ParticleSystem*> s_particles_;

// Initialize physics engine
void initPhysics(bool interactive) {
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	//-----------------Objects----------------

	// Sistema Fuente
	Vector3 pos = GetCamera()->getDir() * 100 + Vector3{100, 0, 0};
	s_particles_.push_back(new ParticleSystem(pos, SystemType::Fountain));

	// Sistema Fireworks
	pos = GetCamera()->getDir() * 100;
	s_particles_.push_back(new ParticleSystem(pos, SystemType::Fireworks));
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t) {
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	if (s_particles_.size() != 0) {
		for (auto s : s_particles_) s->update(t);
	}

	if (particles_.size() > 0) {
		for (auto p : particles_) p->update(t);
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive) {
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	if (particles_.size() > 0) {
		for (auto p : particles_) {
			delete p;
			//p = nullptr;
		}
	}

	delete ParticleSystem_;

	gFoundation->release();
}

void createParticle(Vector3 acc, float dump) {
	PxShape* shape = CreateShape(*Sphere_);
	myParticle = new Particle(shape, color, 1.0f);
	shape->release();

	myParticle->init(
		GetCamera()->getTransform().p,
		{ GetCamera()->getDir() * 100 },
		acc,
		dump);

	particles_.push_back(myParticle);
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera) {
	PX_UNUSED(camera);

	switch(toupper(key)) {
	//case 'B': break;
	//case ' ':	break;
	case 'B': {
		Vector3 acc = {GetCamera()->getDir() * 100};
		createParticle(acc, 0.5f);

		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

int main(int, const char*const*) {
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}