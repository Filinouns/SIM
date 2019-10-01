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

	void explote();
	void update(float t);

	Vector4 randColor();

private:
	bool exploted_;
	int generations_;

	list<Particle*> sub_particles;

	Vector4 red = { 1, 0, 0, 1 };
	Vector4 magenta = { 1, 0, 1, 1 };
	Vector4 blue = { 0, 0, 1, 1 };
	Vector4 yellow = { 1, 0.92, 0.016, 1};
};