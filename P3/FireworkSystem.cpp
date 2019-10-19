#include "FireworkSystem.h"

FireworkSystem::FireworkSystem(Vector3 p) : ParticleSystem(p) {
	countdown_ = 1;
	MAX_PARTICLES = 100;
}

FireworkSystem::~FireworkSystem() {}

//-------------Updates---------------
void FireworkSystem::update(float t) {
	particleGenerator(t);
	
	if (particles_.size() > 0) {
		deleteParticles(t);

		// Actualizamos las particulas
		for (auto p : particles_) p->update(t);
	}
	if (hijos_.size() > 0) {
		for (auto p : hijos_) p->update(t);
	}
}

void FireworkSystem::particleGenerator(float t) {
	countdown_ -= t;

	if (countdown_ <= 0 && n_Particles < MAX_PARTICLES) {
		createParticle(static_cast<int>(rand() % 3));
		countdown_ = iniCount_;
	}
}

void FireworkSystem::deleteParticles(float t) {
	Particle *a;
	n_Particles = hijos_.size() + particles_.size();
	//Comprobamos que las particulas no superan el total permitido
	if (n_Particles >= MAX_PARTICLES) {
		int sobrantes = n_Particles - MAX_PARTICLES;
		for (int i = 0; i < sobrantes; i++) {
			a = *hijos_.begin();
			hijos_.pop_front();
			delete a;
			//n_Particles--;
		}
	}
	// Comprobamos el estado de los padres
	a = *particles_.begin();
	if (a->getState() == State::INVISIBLE) {
		for (int i = 0; i < static_cast<FireWork*>(a)->getGenerations(); i++) {
			createSon(a->getPos());
		}
		//Eliminamos la particula
		particles_.pop_front();
		delete a;
		//n_Particles--;
	}
}

//-----------------Creaciones de particulas-------------------
void FireworkSystem::createSon(Vector3 p) {
	particle = new Particle(CreateShape(PxCapsuleGeometry(1, 2)), rnd_Color(), 10.0f);

	Vector3 vel = { static_cast<float>(rand() % 5),
		40,
		static_cast<float>(rand() % 5) };

	particle->init(p, vel, 0.99);

	particle->setMaxAge(2.0f);

	addForcesToPart(particle);

	n_Particles++;

	hijos_.push_back(particle);
}

void FireworkSystem::createFirework(PxShape* s, Vector3 vel) {
	particle = new FireWork(s, rnd_Color(), 10.0f);

	particle->init(
		pos_.p,
		vel,
		0.99);

	particle->setMaxAge(5.0f);

	addForcesToPart(particle);

	n_Particles++;

	particles_.push_back(particle);
}

void FireworkSystem::createParticle(int t) {
	Vector3 vel;
	switch (t) {
	case Esfera:
		vel = { static_cast<float>(rand() % 11),
			static_cast<float>(rand() % 50 + 25),
			static_cast<float>(rand() % 11) };

		createFirework(CreateShape(PxSphereGeometry(2)), vel);
		break;
	case Capsula:
		vel = { static_cast<float>(rand() % 5),
			40,
			static_cast<float>(rand() % 5) };

		createFirework(CreateShape(PxCapsuleGeometry(1, 2)), vel);
		break;
	case SuperCapsula:
		vel = { static_cast<float>(rand() % 5),
			40,
			static_cast<float>(rand() % 5) };

		createFirework(CreateShape(PxCapsuleGeometry(1.5, 5)), vel);
		break;
	case SuperEsfera:
		vel = { static_cast<float>(rand() % 11),
			static_cast<float>(rand() % 50 + 25),
			static_cast<float>(rand() % 11) };

		createFirework(CreateShape(PxSphereGeometry(3)), vel);
		break;
	default:
		break;
	}
}