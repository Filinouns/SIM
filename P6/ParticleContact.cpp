#include "ParticleContact.h"

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
	// Find the velocity in the direction of the contact.
	float sVel = calculateSeparatingVelocity();

	// Check whether it needs to be resolved.
	if (sVel >= 0) {
		// The contact is either separating or stationary - there’s
		// no impulse required.
		return;
	}
	// Calculate the new separating velocity.
	float newSepVelocity = -sVel * restitution;
	float deltaVelocity = newSepVelocity - sVel;

	// We apply the change in velocity to each object in proportion to
	// its inverse mass (i.e., those with lower inverse mass [higher
	// actual mass] get less change in velocity).
	float totalInverseMass = particle[0]->getMass();
	if (particle[1]) totalInverseMass += particle[1]->getMass();

	// If all particles have infinite mass, then impulses have no effect.
	if (totalInverseMass <= 0) return;
	// Calculate the impulse to apply.
	float impulse = deltaVelocity / totalInverseMass;
	// Find the amount of impulse per unit of inverse mass.
	Vector3 impulsePerIMass = contactNormal * impulse;

	// Apply impulses: they are applied in the direction of the contact,
	// and are proportional to the inverse mass.
	particle[0]->setVel(
		particle[0]->getVel() +	
		impulsePerIMass * particle[0]->getMass()
	);

	if (particle[1]) {
		// Particle 1 goes in the opposite direction.
		particle[1]->setVel(
			particle[1]->getVel() +
			impulsePerIMass * -particle[1]->getMass()
		);
	}
}

void ParticleContact::resolveInterpenetration(float t) { 
	// Check if there is interpenetration, If we don’t have any penetration, skip this step.
	if (penetration <= 0) return;

	// The movement of each object is based on its inverse mass, so
	// total that.
	float totalInverseMass = particle[0]->getMass();
	if (particle[1]) totalInverseMass += particle[1]->getMass();

	// If all particles have infinite mass, then we do nothing.
	if (totalInverseMass <= 0) return;

	// Find the amount of penetration resolution per unit of inverse mass.
	Vector3 movePerIMass = contactNormal *
		(-penetration / totalInverseMass);

	// Apply the penetration resolution.
	particle[0]->setPos(particle[0]->getPos() +
		movePerIMass * particle[0]->getMass());
	if (particle[1]) {
		particle[1]->setPos(particle[1]->getPos() +
			movePerIMass * particle[1]->getMass());
	}
}