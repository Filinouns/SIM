#include "ParticleContactManager.h"

ParticleContactManager::ParticleContactManager() {}
ParticleContactManager::~ParticleContactManager() {}

void ParticleContactManager::update(double t) {
	while (!q.empty()) {
		ParticleContact* pc = q.front(); q.pop();
		pc->resolve(t);
	}
}