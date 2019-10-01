#include "ParticleSystem.h"

PxGeometry*				Geom_	   = NULL;	//Geometria
Particle*				particle = NULL; //Particula

Vector3 gravity = { 0, -10, 0 };

ParticleSystem::ParticleSystem(PxGeometry *g, Vector4 color, Vector3 p, SystemType t) {
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

	color_ = color;
	Geom_ = g;
	pos_.p = p;
}

ParticleSystem::~ParticleSystem() {
	for (auto p : particles_) {
		delete p;
	}
}

void ParticleSystem::createParticle() {
	PxShape* shape = CreateShape(*Geom_);
	particle = new Particle(shape, selectRndColor(), 10.0f);
	shape->release();

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
	PxShape* shape = CreateShape(*Geom_);
	/*
	particle = new FireWork(shape, selectRndColor(), 10.0f);
	shape->release();

	particle->init(
		pos_.p,
		{ static_cast<float>(rand() % 11),
		static_cast<float>(rand() % 50 + 25),
		static_cast<float>(rand() % 11) },
		gravity,
		0.99);
	*/
	particles_.push_back(particle);
}

Vector4 ParticleSystem::selectRndColor() {
	Vector4 c;

	switch (rand() % 3) {
	case 0:
		c = color_;
		break;
	case 1:
		c = color1;
		break;
	case 2:
		c = color2;
		break;
	default:
		c = color_;
		break;
	}
	return c;
}

void ParticleSystem::update(float t) {
	countdown_ -= t;

	if (countdown_ <= 0) {
		if(type_ == 0) createParticle();
		else if (type_ == 1) createFirework();
		countdown_ = iniCount_;
	}

	if (particles_.size() > 0) {
		for (auto p : particles_) {
			p->update(t);
		}
	}
}