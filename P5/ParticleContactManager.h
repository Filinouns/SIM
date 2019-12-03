#pragma once

#include "ParticleContact.h"
#include <queue>

using namespace std;

class ParticleContactManager {
public:
	ParticleContactManager();
	virtual ~ParticleContactManager();

	virtual void update(double t);
	void addContact(ParticleContact* pc) { q.push(pc); };

private:
	queue<ParticleContact*> q;
};