#include "ParticleRod.h"

unsigned ParticleRod::addContact(ParticleContact* contact) const {
	// Find the length of the rod.
	float currentLen = currentLength();

	// Check if we’re overextended or overcompressed
	if (currentLen == length) return 0;

	// Otherwise return the contact updated.
	contact->particle[0] = particle[0];
	contact->particle[1] = particle[1];

	// Calculate the normal.
	Vector3 normal = particle[1]->getPos() - particle[0]->getPos();
	normal.normalize();

	// The contact normal depends on whether we’re extending or compressing.
	if (currentLen > length) {
		contact->contactNormal = normal;
		contact->penetration = currentLen - length;
	} else {
		contact->contactNormal = normal * -1;
		contact->penetration = length - currentLen;
	}

	// Zero restitution (no bounciness).
	contact->restitution = 0;
	return 1;
}