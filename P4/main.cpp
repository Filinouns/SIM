
#include <iostream>

//----------------------------------Includes----------------------------------
#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <list>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

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

using namespace physx;
using namespace std;

//---------------------------------------------------------------------------

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;

PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback	gContactReportCallback;

//Muelles
Particle*			   	myParticle	= NULL; //Particula
Particle*				Cube_		= NULL;

//Fuerzas
ParticleGravity*		grav1		= NULL;	//Gravedad1
ParticleGravity*		grav2		= NULL;	//Gravedad2
Wind*					wind		= NULL;	//Viento
Wind*					wind1		= NULL;	//Viento temp para muelles
Explosion*				explosion	= NULL; //Explosion
//ParticleSpring*			muelle		= NULL;	//Muelle Comun
ParticleAnchoredSpring*	muelleFijo	= NULL;	//Punto de anclaje del muelle

ParticleForceRegistry*	reg			= NULL;		// Registro de fuerzas para los sistemas
		
vector<vector<ParticleForceGenerator*>> forces_;	// Vector de fuerzas para cada sistema
list<ParticleSystem*> s_particles_;					// Lista de sistemas de particulas
list<Particle*> particles_;

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

	Vector3 pos; 		

	//-----------------Objects----------------
	// Plano donde reposaran los demas elementos de la escena
	Vector4 color = { 1, 0, 1, 1 };
	Particle* plano = new Particle(CreateShape(*new PxBoxGeometry(100, 1, 1000)), { 0, 0, 0, 1 }, 0);
	plano->setPos({ 0, 0, 0 });

	// Sistema Fuente
	pos = GetCamera()->getDir() * 100 + Vector3{ 200, 100, 0 };
	ParticleSystem* FountainSystem = new ParticleSystem(pos);
	s_particles_.push_back(FountainSystem);

	// Sistema Fireworks
	pos = GetCamera()->getDir() * 100 + Vector3{ -100, 400, 0};
	FireworkSystem* fireWorkSystem = new FireworkSystem(pos);
	s_particles_.push_back(fireWorkSystem);

	//----------------------------Muelles------------------------------
	//-----------P1
	//Punto anclaje
	pos = GetCamera()->getDir() * 100 + Vector3{ 200, 100, 200 };
	Cube_ = new Particle(CreateShape(*new PxBoxGeometry(5, 5, 5)), color, 1);
	Cube_->init(pos, { 0, 0, 0 }, 0.99);
	Cube_->setState(INF);
	particles_.push_back(Cube_);
	//Particula anclada
	myParticle = new Particle(CreateShape(*new PxSphereGeometry(3)), { 0, 0, 1, 1 }, 2.0f);
	myParticle->init(
		Cube_->getPos() - Vector3{ 0, 20, 0 },
		{0, 0, 0},
		1);
	myParticle->setState(INF);
	particles_.push_back(myParticle);
	//-----------P2
	pos = GetCamera()->getDir() * 100 + Vector3{ -200, 100, 200 };
	//Particula1
	Particle* PartP2_1 = new Particle(CreateShape(*new PxSphereGeometry(5)), { 1, 0.92, 0.016, 1 }, 2.0f);
	PartP2_1->init(pos, { 0, 0, 0 }, 0.99);
	PartP2_1->setState(INF);
	particles_.push_back(PartP2_1);
	//Particula2
	Particle* PartP2_2 = new Particle(CreateShape(*new PxSphereGeometry(5)), { 1, 0.92, 0.016, 1 }, 2.0f);
	PartP2_2->init(pos + Vector3{ 0, 50, 0 }, { 0, 0, 0 }, 0.99);
	PartP2_2->setState(INF);
	particles_.push_back(PartP2_2);
	//-----------P3
	//Particula
	pos = GetCamera()->getDir() * 100 + Vector3{200, 100, 300};
	Particle* PartP3_1 = new Particle(CreateShape(*new PxBoxGeometry(5, 5, 5)), color, 20);
	PartP3_1->init(pos, { 0, 0, 0 }, 0.99);
	PartP3_1->setState(INF);
	particles_.push_back(PartP3_1);
	//Plano
	Particle* Agua = new Particle(CreateShape(*new PxBoxGeometry(30, 1, 30)), {0, 0, 1, 1}, 1);
	Agua->init(pos - Vector3{ 0, 50, 0 }, { 0, 0, 0 }, 1);
	Agua->setState(INF);
	particles_.push_back(Agua);


	//---------------------------------------Fuerzas---------------------------------
	//--------Creacion de las fuerzas--------
	//------Sistema1 -> Viento -------
	grav1 = new ParticleGravity({ 0, -10, 0 });		// Gravedad
	pos = GetCamera()->getDir() * 100 + Vector3{ 200, 200, 0 };
	wind = new Wind({ 500, 0,0 }, pos, 50);			// Viento
	//------Sistema2 -> Explosion ------
	grav2 = new ParticleGravity({ 0, -35, 0 });		// Gravedad2
	pos = GetCamera()->getDir() * 100 + Vector3{ -100, 150, 0 };
	explosion = new Explosion(100000, pos, 100);	// Explosion
	//-Muelles-
	//P1
	muelleFijo = new ParticleAnchoredSpring(&Cube_->getPos(), 1, 20);	// Muelle anclado
	//P2
	ParticleSpring* muelle = new ParticleSpring(PartP2_1, 1, 30);
	ParticleSpring* muelle1 = new ParticleSpring(PartP2_2, 1, 30);
	//P3
	ParticleBuoyancy* water = new ParticleBuoyancy(5, 1.1f, Agua->getPos().y, 1000.0f);

	// Registro de fuerzas
	reg = new ParticleForceRegistry();

	// Vector de Generadores de Fuerzas, cada "x" son las fuerzas que se aplican a 1 unico sistema
	forces_.resize(s_particles_.size());
	//Fuerzas del sistema fuente
	forces_[0].push_back(grav1);
	forces_[0].push_back(wind);
	//Fuerzas del sistema Fireworks
	forces_[1].push_back(grav2);
	forces_[1].push_back(explosion);
	//Fuerzas muelles
	//forces_[2].push_back(grav1);
	//forces_[2].push_back(muelleFijo);

	// Dar fuerzas a los sistemas
	int i = 0;
	for (auto s : s_particles_) {
		s->setReg(reg);
		for (auto f : forces_[i]) s->registerForce(f);
		i++;
	}

	// Fuerzas de los muelles
	// P1 muelles
	muelleFijo->setAnchor(&Cube_->getPos());
	reg->add(myParticle, muelleFijo);
	reg->add(myParticle, grav1);
	// P2 muelles
	reg->add(PartP2_2, muelle);
	reg->add(PartP2_1, muelle1);
	// P3 muelles
	reg->add(PartP3_1, water);
	reg->add(PartP3_1, grav2);
	
	//--------------CAMARA------------------
	//myCamera_ = GetCamera();

}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t) {
	//std::cout << "Anchor main: " << muelleFijo->getAnchor().x << ", " << muelleFijo->getAnchor().y << ", " << muelleFijo->getAnchor().z << "\n";
	// Necesario pasarselo por aqui todo el rato porque sino se vuelve loco porque quiere :(
	muelleFijo->setAnchor(&Cube_->getPos());
	//std::cout << "Anchor main: " << muelleFijo->getAnchor().x << ", " << muelleFijo->getAnchor().y << ", " << muelleFijo->getAnchor().z << "\n";
	
	PX_UNUSED(interactive);

	gScene->simulate(t);
	gScene->fetchResults(true);

	// Update Fuerzas, revisar porque las fuerzas dejan de actualizarse al rato ;)
	reg->UpdateForces(t);

	// Update Sistemas
	if (s_particles_.size() != 0) {
		for (auto s : s_particles_) s->update(t);
	}

	//muelle->updateForce(PartP2_2, t);
	if (particles_.size() != 0) {
		for (auto p : particles_) p->update(t);
	}

	// Limpieza del viento1 para que no interactue con la particula.
	if (wind1 != NULL && wind1->getRender()) {
		wind1->unrender();
		reg->remove(myParticle, wind1);
		delete wind1;
		wind1 = NULL;
	}
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive) {
	PX_UNUSED(interactive);

	// ------Limpieza de los elementos de la escena-------
	// Limpieza de los Sistemas de particulas
	if (s_particles_.size() > 0) {
		for (auto p : s_particles_) {
			delete p;
			p = nullptr;
		}
	}

	// Limpieza de las Fuerzas
	if (forces_.size() > 0) {
		for (auto i : forces_) {
			for (auto f : i) {
				delete f;
				f = nullptr;
			}
		}
	}

	// Limpieza de particulas sueltas
	if (particles_.size() > 0) {
		for (auto p : particles_) {
			delete p;
			p = nullptr;
		}
	}

	// Limpieza de elementos sueltos
	if (wind1 != NULL) {
		delete wind1;
		wind1 = nullptr;
	}
	delete muelleFijo;
	muelleFijo = nullptr;

	reg->clear();
	delete reg;
	reg = nullptr;

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera) {
	PX_UNUSED(camera);

	switch(toupper(key)) {
	//Activar la explosion
	case 'B': {
		explosion->activate(true);
		break;
	}
	// Desactivar la fuerza afectando al sistema de muelles
	case char(32): {
		if(wind1 == nullptr) wind1 = new Wind({ 300, 0, -100 }, Cube_->getPos(), 50);
		else wind1->unrender();

		reg->add(myParticle, wind1);
		break;
	}
	// Aumentar la k
	case '+':
		muelleFijo->addK();
		break;
	// Disminuir la k
	case '-':
		muelleFijo->subK();
		break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2) {
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

void createParticle(Vector3 pos) {

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