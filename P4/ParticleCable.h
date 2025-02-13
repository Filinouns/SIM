#pragma once
#include "ParticleLink.h"
class ParticleCable : public ParticleLink {
public:
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;

	float maxLength;
	// Holds the restitution (bounciness) of the cable.
	float restitution;
};