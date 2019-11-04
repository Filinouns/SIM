#include "ParticleForceRegistry.h"

ParticleForceRegistry::ParticleForceRegistry() {
	registrations.capacity();
}
ParticleForceRegistry::~ParticleForceRegistry() {}

void ParticleForceRegistry::add(Particle* p, ParticleForceGenerator* fg) {
	ParticleForceRegistration newReg(p, fg);
	registrations.push_back(newReg);
}

void ParticleForceRegistry::remove(Particle* p, ParticleForceGenerator* fg) {
	int i = 0;
	bool killed = false;
	while (i != registrations.size() && !killed) {
		if (registrations[i].fg_ == fg && registrations[i].part_ == p) {
			registrations.erase(registrations.begin() + i);
			killed = true;
		}
		i++;
	}
}

void ParticleForceRegistry::clear() {
	registrations.clear();
}

void ParticleForceRegistry::UpdateForces(float t) {
	for (auto it = registrations.begin(); it != registrations.end(); it++) {
		it->fg_->updateForce(it->part_, t);
	}
}