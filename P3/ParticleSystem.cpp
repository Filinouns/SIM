#include "ParticleSystem.h"

//------------------Constructor_Destructor-------------
ParticleSystem::ParticleSystem(Vector3 p) {
	countdown_ = .5;
	iniCount_ = countdown_;

	pos_.p = p;

	n_Particles = 0;
}

ParticleSystem::~ParticleSystem() {
	for (auto p : particles_) {
		delete p;
	}
}

//--------------------------Updates----------------------
void ParticleSystem::update(float t) {
	particleGenerator(t);

	if (particles_.size() > 0) {
		deleteParticles(t);

		// Actualizamos las particulas
		for (auto p : particles_) {
			p->update(t);
		}
	}
}

void ParticleSystem::deleteParticles(float t) {
	Particle *a = *particles_.begin();
	//Comprobamos que las particulas no estan desactivadas o que no superamos el total permitido
	while (a->getState() == State::OFF || n_Particles >= MAX_PARTICLES) {
		particles_.pop_front();
		delete a;
		n_Particles--;
		a = *particles_.begin();
	}
}

void ParticleSystem::particleGenerator(float t) {
	countdown_ -= t;

	if (countdown_ <= 0 && n_Particles < MAX_PARTICLES) {
		createParticle();
		n_Particles++;
		countdown_ = iniCount_;
	}
}


//-------------------------Diverse Metods-----------------
void ParticleSystem::createParticle() {
	particle = new Particle(CreateShape(*new PxSphereGeometry(2)), rnd_Color(), 10.0f);

	particle->init(
		pos_.p, 
		{ static_cast<float>(rand() % 11),
		static_cast<float>(rand() % 50 + 25),
		static_cast<float>(rand() % 11) },
		0.99);

	addForcesToPart(particle);

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

//System Forces
void ParticleSystem::registerForce(ParticleForceGenerator * fg) {
	forces_.push_back(fg);
}

void ParticleSystem::addForcesToPart(Particle* p) {
	if (forces_.size() != 0) {
		for (int i = 0; i < forces_.size(); i++) {
			regist->add(p, forces_[i]);
		}
	}
}