#pragma once
#include "ParticleForceGenerator.h"
#include <complex>
class ParticleSpring :
	public ParticleForceGenerator
{
private:
	Particula* other;
	RigidObject* otherRigid;

	float k;
	float restLength;

	Vector3* anchor;
	PxTransform trans;

	Vector4 myColor;

	RenderItem* cube;

public:
	ParticleSpring(float k_, float restLength_, Vector3* anchor_, Particula* other = nullptr);
	ParticleSpring(float k_, float restLength_, Vector3* anchor_, RigidObject* other_ = nullptr);

	virtual void updateForce(Particula* particle, float t);
	virtual void updateForce(RigidObject* particle, float t);

	float getConstant() {
		return k;
	}

	void setConstant(float k_) {
		k += k_;
	}

	Vector3 getPos() {
		return cube->transform->p;
	}

	RenderItem* getCube() {
		return cube;
	}

	virtual ~ParticleSpring();
};

