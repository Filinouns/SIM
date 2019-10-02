#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(Vector3 p, SystemType t) {
	type_ = t;
	switch (type_) {
	case Fountain:
		countdown_ = 0.1;
		break;
	case Fireworks:
		countdown_ = 1;
		break;
	default:
		countdown_ = 0.5;
		break;
	}
	iniCount_ = countdown_;

	pos_.p = p;

	n_Particles = 0;
}

ParticleSystem::~ParticleSystem() {
	for (auto p : particles_) {
		delete p;
	}
}

void ParticleSystem::createParticle() {
	particle = new Particle(CreateShape(*new PxSphereGeometry(2)), rnd_Color(), 10.0f);

	particle->init(
		pos_.p, 
		{ static_cast<float>(rand() % 11),
		static_cast<float>(rand() % 50 + 25),
		static_cast<float>(rand() % 11) },
		gravity,
		0.99);

	particles_.push_back(particle);
}

void ParticleSystem::createFirework() {
	PxShape* shape = CreateShape(PxCapsuleGeometry(1, 2));
	// Preguntar como rotarla aqui, creo que se hace en el pxTransform de partícula
	particle = new FireWork(shape, rnd_Color(), 10.0f);
	shape->release();

	particle->init(
		pos_.p,
		{ static_cast<float>(rand() % 5),
		40,
		static_cast<float>(rand() % 5) },
		gravity,
		0.99);

	particle->setMaxAge(5.0f);

	particles_.push_back(particle);
}

void ParticleSystem::createParticle(int t) {
	switch (t) {
	case Esfera:
		particle = new FireWork(CreateShape(PxSphereGeometry(2)), rnd_Color(), 10.0f);

		particle->init(
			pos_.p,
			{ static_cast<float>(rand() % 11),
			static_cast<float>(rand() % 50 + 25),
			static_cast<float>(rand() % 11) },
			gravity,
			0.99);
		particle->setMaxAge(5.0f);
		break;
	case Capsula:
		particle = new FireWork(CreateShape(PxCapsuleGeometry(1, 2)), rnd_Color(), 10.0f);

		particle->init(
			pos_.p,
			{ static_cast<float>(rand() % 5),
			40,
			static_cast<float>(rand() % 5) },
			gravity,
			0.99);

		particle->setMaxAge(5.0f);
		break;
	case SuperCapsula:
		particle = new FireWork(CreateShape(PxCapsuleGeometry(1.5, 5)), rnd_Color(), 10.0f);

		particle->init(
			pos_.p,
			{ static_cast<float>(rand() % 5),
			40,
			static_cast<float>(rand() % 5) },
			gravity,
			0.99);

		particle->setMaxAge(5.0f);
		break;
	case SuperEsfera:
		particle = new FireWork(CreateShape(PxSphereGeometry(3)), rnd_Color(), 10.0f);

		particle->init(
			pos_.p,
			{ static_cast<float>(rand() % 11),
			static_cast<float>(rand() % 50 + 25),
			static_cast<float>(rand() % 11) },
			gravity,
			0.99);
		particle->setMaxAge(5.0f);
		break;
	default:
		break;
	}

	particles_.push_back(particle);
}

Vector4 ParticleSystem::rnd_Color() {
	Vector4 c;
	switch (rand() % 6) {
	case 0:
		c = red;
		break;
	case 1:
		c = magenta;
		break;
	case 2:
		c = blue;
		break;
	case 3:
		c = yellow;
		break;
	case 4:
		c = color1;
		break;
	case 5:
		c = color2;
		break;
	default:
		c = red;
		break;
	}
	return c;
}

void ParticleSystem::update(float t) {
	countdown_ -= t;

	if (countdown_ <= 0 && n_Particles < MAX_PARTICLES) {
		if (type_ == Fountain) createParticle(); 
		else if (type_ == Fireworks) createParticle(static_cast<int>(rand() % 3));
		n_Particles++;
		countdown_ = iniCount_;
	}

	if (particles_.size() > 0) {
		Particle *a = *particles_.begin();
		//Comprobamos que las particulas no estan desactivadas
		while (a->getState() == State::OFF) {
			particles_.pop_front();
			delete a;
			n_Particles--;
			a = *particles_.begin();
		}

		//Comprobamos el numero total de particulas
		if (n_Particles >= MAX_PARTICLES) {
			a = *particles_.begin();
			particles_.pop_front();
			delete a;
			n_Particles--;
		}

		// Actualizamos las que quedan
		for (auto p : particles_) {
			p->update(t);
		}
	}
}