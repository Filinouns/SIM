#include "ParticleContactResolver.h"

ParticleContactResolver::ParticleContactResolver(unsigned iterations) : iterations_(iterations) {}

void ParticleContactResolver::resolveContacts(ParticleContact* contactArray, unsigned numContacts, float duration) {
	iterationsUsed = 0;
	while (iterationsUsed < iterations_)	{
		// Find the contact with the largest closing velocity;
		float max = 0;
		unsigned maxIndex = numContacts;
		for (unsigned i = 0; i < numContacts; i++) {
			float sepVel = contactArray[i].calculateSeparatingVelocity();
			if (sepVel < max) {
				max = sepVel;
				maxIndex = i;
			}
		}
		// Resolve this contact.
		contactArray[maxIndex].resolve(duration);
		iterationsUsed++;
	}
}