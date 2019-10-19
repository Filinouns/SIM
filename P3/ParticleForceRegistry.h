#pragma once
#include "Particle.h"
#include "ParticleForceGenerator.h"

class ParticleForceRegistry {
protected:
	struct ParticleForceRegistration {
		Particle* part_;
		ParticleForceGenerator* fg_;

		ParticleForceRegistration(Particle* part, ParticleForceGenerator* fg) {
			part_ = part;
			fg_ = fg;
		}
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

public:
	ParticleForceRegistry();
	virtual ~ParticleForceRegistry();

	// Asociar un generador a una particula
	void add(Particle* p, ParticleForceGenerator* fg);

	// Quitar una asociacion
	void remove(Particle* p, ParticleForceGenerator* fg);

	// Quita todas las asociaciones. Las particulas y los generadores no seran destruidos.
	void clear();

	// Update de todos los generadores del registro
	void UpdateForces(float t);
};