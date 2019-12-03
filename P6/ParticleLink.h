#pragma once
#include "ParticleContact.h"

class ParticleLink {
public:
	//Holds the pair of particles that are connected by this link.
	Particle* particle[2];
	
	virtual unsigned addContact(ParticleContact* contact) const = 0;
protected:
	float currentLength() const;
};

