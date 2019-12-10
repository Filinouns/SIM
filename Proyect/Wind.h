#pragma once
#include "ParticleForceGenerator.h"
#include "RenderUtils.hpp"
class Wind :
	public ParticleForceGenerator
{
public:
	Wind(const Vector3& vient, Vector3 posicion = { 0, 0, 0 }, int r = 30);
	virtual ~Wind();
	virtual void updateForce(Particula* particle, float t);
	virtual void updateForce(RigidObject* particle, float t);
	int getRadio() {
		return sphRadio;
	}

	RenderItem* getSphera() {
		return sphera;
	}

private:
	Vector3 myPos;
	PxTransform trans;
	Vector3 viento;
	Vector4 myColor;
	RenderItem* sphera;
	int sphRadio;
};