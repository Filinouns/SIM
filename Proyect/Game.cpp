#include "Game.h"



Game::Game(physx::PxScene* gScene_, physx::PxPhysics* gPhysics_) : gScene(gScene_), gPhysics(gPhysics_){
	
	/*
	pFRegRigids = new ParticleForceRegistry<RigidObject>();

	rb = new RigidBody(gScene, gPhysics);
	rb->getReg(pFRegRigids);

	explosion = new Explosion(100, 10, { 0, 0, 0 }, 40.0f);
	wind = new Wind({ -10, 0, 0 }, { 0, 0, 0 }, 30.0f);

	fManager = new FireworkManager();
	fManager->getReg(pFRegParticulas);

	rb->addForceToGens(explosion);
	rb->addForceToGens(wind);
	*/

	init();
}


Game::~Game(){
	/*delete fManager;
	fManager = nullptr;

	delete wind;
	wind = nullptr;

	delete explosion;
	explosion = nullptr;

	delete rb;
	rb = nullptr;

	delete pFRegRigids;
	pFRegRigids = nullptr;*/

	delete bullets;
	bullets = nullptr;

	delete targets;
	targets = nullptr;
}

void Game::init() {
	//Colocamos la camara
	GetCamera()->setPos(GetCamera()->getEye() + PxVec3{ 0, 5, 0 });

	bullets = new RigidBody(gScene, gPhysics);
	targets = new RigidBody(gScene, gPhysics, true);
}

void Game::update(double t) {
	//rb->update(t);
	//fManager->FireworksUpdate(t);
	//pFRegRigids->updateForces(t);
	bullets->update(t);
	targets->update(t);
	checkColissions();
}

void Game::keyPress(unsigned char key) {
	switch (toupper(key)) {
		//case 'B': break;
		//case ' ':	break;
	case ' ': {
		GetCamera()->setPos(GetCamera()->getEye() + PxVec3{ 0, 5, 0 });
		break;
	}
	case 'P': {
		bullets->creaBala(GetCamera()->getTransform().p, GetCamera()->getDir());
		break;
	}
	case 'X': {
		//rb->creaRigidDynamic();
		break;
	}
	case 'C': {
		targets->creaObjetivo();
		break;
	}
	case 'V': {
		//rb->creaSuelo();
		break;
	}
	case 'B': {
		//rb->setActivo();
		break;
	}
	case 'N': {
		//wind->getSphera()->setActivo();
		break;
	}
	case 'M': {
		//explosion->getSphera()->setActivo();
		break;
	}
	case 'E': {
		//explosion->setActive();
		break;
	}
	case 'F': {
		//fManager->fireWorksCreate(azul);
		break;
	}
	default:
		break;
	}
}

void Game::checkColissions() {
	for (auto bullet : bullets->getBodies()) {
		for (auto target : targets->getBodies()) {
			if (bullet != nullptr) {
				if (target != nullptr) {
					if (bullet->getBody()->getWorldBounds().intersects(target->getbodySt()->getWorldBounds())) {
						bullet->setCanDisappear(true);
						gScene->removeActor(*bullet->getBody());
						target->setCanDisappear(true);
						gScene->removeActor(*target->getbodySt());
					}
				}
			}
		}
	}
}