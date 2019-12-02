#include "ParticleLink.h"

float ParticleLink::currentLength() const {
	Vector3 relPos = particle[0]->getPos() - particle[1]->getPos();
	return relPos.magnitude();
}