#pragma once
#include "ParticleForceGenerator.h"

template <typename T>
class ParticleForceRegistry {
protected:
	struct ParticleForceRegistration {
		T* particle;
		ParticleForceGenerator* fg;

		ParticleForceRegistration(T* part, ParticleForceGenerator* gen) {
			particle = part;
			fg = gen;
		}

		T* getPart() {
			return particle;
		}

		ParticleForceGenerator* getFGen() {
			return fg;
		}
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

public:
	ParticleForceRegistry() {};
	virtual ~ParticleForceRegistry() {};

	void add(T* particle, ParticleForceGenerator* fg) {
		ParticleForceRegistration newReg(particle, fg);
		registrations.push_back(newReg);
	};

	void remove(T* particle, ParticleForceGenerator* fg) {
		int i = 0;
		while (i <= registrations.size()) {
			if (registrations[i].getFGen() == fg && registrations[i].getPart() == particle) {
				registrations.erase(registrations.begin() + i);
			}
			i++;
		}
	};

	void clear() {
		registrations.clear();
	};

	void updateForces(float t) {
		for (int i = 0; i < registrations.size(); i++) {
			registrations[i].fg->updateForce(registrations[i].particle, t);
		}
	};
};