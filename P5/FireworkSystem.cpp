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
		for (auto p : particles_) {
			if (p->getState() == ON) {
				Vector3 FatherPos = p->getPos();
				int gen = static_cast<FireWork*>(p)->getGenerations();
				p->update(t);
				if (p->getState() == OFF) {
					DeregisterRenderItem(p);
					for (int i = 0; i < gen; i++) {
						createSon(FatherPos);
					}
				}
			}
		}
	}
}

void FireworkSystem::particleGenerator(float t) {
	countdown_ -= t;

	if (countdown_ <= 0) {
		createParticle(static_cast<int>(rand() % 3));
		countdown_ = iniCount_;
	}
}

//-----------------Creaciones de particulas-------------------
void FireworkSystem::createSon(Vector3 p) {
	firework = new FireWork(CreateShape(PxCapsuleGeometry(1, 2)), rnd_Color(), 10.0f);

	float vx, vz;
	vx = rand() % 2;
	vz = rand() % 2;
	if (vx == 0) vx = -1;
	if (vz == 0) vz = -1;

	Vector3 vel = { static_cast<float>(rand() % 45) * vx,
		40,
		static_cast<float>(rand() % 45) * vz };

	firework->init(p, vel, 0.99);

	firework->setMaxAge(2.0f);
	firework->setGen(0);

	addForcesToPart(firework);

	particles_.push_back(firework);
}

void FireworkSystem::createFirework(PxShape* s, Vector3 vel) {
	particle = new FireWork(s, rnd_Color(), 10.0f);

	particle->init(
		pos_.p,
		vel,
		0.99);

	particle->setMaxAge(5.0f);

	addForcesToPart(particle);

	particles_.push_back(particle);
}

void FireworkSystem::createParticle(int t) {
	float vx, vz;
	vx = rand() % 2;
	vz = rand() % 2;
	if (vx == 0) vx = -1;
	if (vz == 0) vz = -1;

	Vector3 vel;
	switch (t) {
	case Esfera:
		vel = { static_cast<float>(rand() % 11) * vx,
			static_cast<float>(rand() % 20 + 25),
			static_cast<float>(rand() % 11) * vz};

		createFirework(CreateShape(PxSphereGeometry(2)), vel);
		break;
	case Capsula:
		vel = { static_cast<float>(rand() % 10) * vx,
			40,
			static_cast<float>(rand() % 10) * vz};

		createFirework(CreateShape(PxCapsuleGeometry(1, 2)), vel);
		break;
	case SuperCapsula:
		vel = { static_cast<float>(rand() % 10) * vx,
			40,
			static_cast<float>(rand() % 10) * vz};

		createFirework(CreateShape(PxCapsuleGeometry(1.5, 5)), vel);
		break;
	case SuperEsfera:
		vel = { static_cast<float>(rand() % 11) * vx,
			static_cast<float>(rand() % 20 + 25),
			static_cast<float>(rand() % 11) * vz };

		createFirework(CreateShape(PxSphereGeometry(3)), vel);
		break;
	default:
		break;
	}
}