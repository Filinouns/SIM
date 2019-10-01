#pragma once

#include "Particle.h"

class FireWork :
	public Particle {
public:
	FireWork(PxShape* s, Vector4 color, float m);
	virtual ~FireWork();

	//void init(Vector3 p, Vector3 v, Vector3 acc);

private:
	float explote_;
	int generations_;
};

