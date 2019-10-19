#pragma once
#include "ParticleForceGenerator.h"
class Wind : public ParticleForceGenerator {

public:
	Wind(const Vector3& vient, Vector3 posicion, int r);
	virtual ~Wind();

	virtual void updateForce(Particle* p, float t);

	int getR() { return r_; }
private:
	Vector3 pos_;
	PxTransform trans_;
	Vector3 wind_;
	Vector4 color_;
	RenderItem* sphere_;
	int r_;
};

