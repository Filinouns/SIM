#include "ParticleContact.h"

ParticleContact::ParticleContact() {

}


ParticleContact::~ParticleContact() {}

void ParticleContact::resolve(float t) {
	resolveVelocity(t); 
	resolveInterpenetration(t);
}

float ParticleContact::calculateSeparatingVelocity() const {
	Vector3 relativeVelocity = particle[0]->getVel(); 
	if (particle[1]) relativeVelocity -= particle[1]->getVel(); 
	return (relativeVelocity.x * contactNormal.x + relativeVelocity.y * contactNormal.y + relativeVelocity.z * contactNormal.z);
}

void ParticleContact::resolveVelocity(float t) {
	// Get current separating velocity
	float sVel = calculateSeparatingVelocity();
	// Compute separating velocity after collision

	// Compute total impulse

	// Compute impulse for each particle

	// Set new velocity for each particle

}

void ParticleContact::resolveInterpenetration(float t) { 
	// Check if there is interpenetration
	//penetration = particle[0]->getPos() - particle[1]->getPos();
	if (penetration < 0) return;
	// Calculate factor of each particle depending on mass

	// Calculate the movement for each particle

	// Apply movement to the position of each particle

}
