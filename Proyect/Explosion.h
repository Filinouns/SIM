#pragma once
#include "ParticleForceGenerator.h"
#include <iostream>
#include "RigidObject.h"
class Explosion :
	public ParticleForceGenerator
{
public:
	Explosion(float fuerza, float timeToDisappear, Vector3 pos = {0, 0, 0}, int r = 30);
	virtual ~Explosion();

	virtual void updateForce(Particula* particle, float t);
	virtual void updateForce(RigidObject* particle, float t);
	int getRadio() {
		return sphRadio;
	}
	void setActive() {
		if (!activo) {
			activo = true;
		}
	}
	bool getActivo() {
		return activo;
	}
	bool activo;

	RenderItem* getSphera() {
		return sphera;
	}

private:
	Vector3 myPos;
	PxTransform trans;
	float myFuerza;
	Vector4 myColor;
	RenderItem* sphera;
	int sphRadio;

	float secondsPassed;
	float timeExpl;
	float nextTime;

	bool puedoBorrar;
};

