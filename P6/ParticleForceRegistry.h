#pragma once
#include "ParticleForceGenerator.h"

template <typename T>
class ParticleForceRegistry {
protected:
	struct ParticleForceRegistration {
		T* part_;
		ParticleForceGenerator* fg_;

		ParticleForceRegistration(T* part, ParticleForceGenerator* fg) {
			part_ = part;
			fg_ = fg;
		}
	};

	typedef std::vector<ParticleForceRegistration> Registry;
	Registry registrations;

public:
	// Asociar un generador a una particula
	void add(T* p, ParticleForceGenerator* fg) {
		ParticleForceRegistration newReg(p, fg);
		registrations.push_back(newReg);
	}

	// Quitar una asociacion
	void remove(T* p, ParticleForceGenerator* fg) {
		int i = 0;
		bool killed = false;
		while (i != registrations.size() && !killed) {
			if (registrations[i].fg_ == fg && registrations[i].part_ == p) {
				registrations.erase(registrations.begin() + i);
				killed = true;
			}
			i++;
		}
	}

	// Quita todas las asociaciones. Las particulas y los generadores no seran destruidos.
	void clear() {
		registrations.clear();
	}

	// Update de todos los generadores del registro
	void UpdateForces(float t) {
		for (auto it = registrations.begin(); it != registrations.end(); it++) {
			it->fg_->updateForce(it->part_, t);
		}
	}
};