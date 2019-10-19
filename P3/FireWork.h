#pragma once

#include "Particle.h"

#include <list>
#include <random>

using namespace std;

class FireWork :
	public Particle {
public:
	FireWork(PxShape* s, Vector4 color, float m);
	virtual ~FireWork();

	void update(float t);

	bool exploted_;
	void setDetected(bool b) { detected_ = b; }
	int getGenerations() { return generations_; }

private:
	int generations_;
	bool detected_;
};