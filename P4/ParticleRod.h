#pragma once
#include "ParticleLink.h"
class ParticleRod :	public ParticleLink {
public:
	virtual unsigned addContact(ParticleContact* contact, unsigned limit) const;
	
	float length;
};

