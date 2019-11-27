#pragma once
#include "ParticleForceGenerator.h"
class Explosion : public ParticleForceGenerator {
public:
	Explosion(float f, Vector3 p, int r);
	virtual ~Explosion();

	virtual void updateForce(Particle* p, double t);

	int getR() { return r_; }
	float setCd(float c) { cd_ = c; }
	void activate(bool b) { 
		active_ = b;
		if (active_) RegisterRenderItem(sphere_);
		else DeregisterRenderItem(sphere_);
	}
	bool isActive() { return active_; }

private:
	Vector3 pos_;
	PxTransform trans_;
	float explosion_;	//Vector para la fuerza de la expolosion
	Vector4 color_;
	RenderItem* sphere_;
	int r_;

	float timePassed_, timeWorking_;
	float cd_;

	bool active_;
};

