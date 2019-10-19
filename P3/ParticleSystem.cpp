#include "ParticleSystem.h"

//------------------Constructor_Destructor-------------
ParticleSystem::ParticleSystem(Vector3 p) {
	countdown_ = .5;
	iniCount_ = countdown_;

	pos_.p = p;
}

ParticleSystem::~ParticleSystem() {
	for (auto p : particles_) delete p;
}

//--------------------------Updates----------------------
void ParticleSystem::update(float t) {
	particleGenerator(t);

	if (particles_.size() > 0) {
		// Actualizamos las particulas
		for (auto p : particles_) 
			if (p->getState() == ON) {
				p->update(t);
				if (p->getState() == OFF) DeregisterRenderItem(p);
			}
	}
}

void ParticleSystem::particleGenerator(float t) {
	countdown_ -= t;

	if (countdown_ <= 0) {
		createParticle();
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

	particle->setMaxAge(5);

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