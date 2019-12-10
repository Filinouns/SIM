#pragma once
#include "ParticleForceGenerator.h"
class ParticleBuoyancy :
	public ParticleForceGenerator
{
private:
	float maxDepth;
	float volume;
	float waterHeight;
	float liquidDensity;
	PxTransform trans;
	Vector3 myPos;
	RenderItem* cube;
	Vector4 myColor;
public:
	ParticleBuoyancy(float _maxDep, float _volume, float _waterHeight, float _liquidDensity = 1000.0f, const Vector3 & pos = {0, 0, 0});
	virtual void updateForce(Particula* particle, float t);
	virtual void updateForce(RigidObject* particle, float t);
	Vector3 getPos() {
		return cube->transform->p;
	}
	void setVolume(float vol) {
		volume += vol;
	}
	float getVolume() {
		return volume;
	}
	RenderItem* getCube() {
		return cube;
	}
	virtual ~ParticleBuoyancy() {
		cube->release();
		delete cube;
		cube = nullptr;
	};
};

