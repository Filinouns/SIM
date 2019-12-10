#include "ParticleBuoyancy.h"



ParticleBuoyancy::ParticleBuoyancy(float _maxDep, float _volume, float _waterHeight, float _liquidDensity, const Vector3 & pos): myPos(pos), trans(myPos), maxDepth(_maxDep), volume(_volume), waterHeight(_waterHeight), liquidDensity(_liquidDensity)
{
	myColor = { 0, 0, 0, 1 };
	cube = new RenderItem(CreateShape(*new PxBoxGeometry(15, waterHeight, 15)), myColor);
	cube->color = Vector4(0.0, 0.0, 1.0, 0.0);
	trans.p = { 0, 0, 0 };
	cube->transform = &trans;
	
	RegisterRenderItem(cube);
	cube->addReference();
}

void ParticleBuoyancy::updateForce(Particula * particle, float t)
{
	float depth = particle->getPos().y;

	if (depth >= (waterHeight + maxDepth)) {
		return;
	}

	Vector3 f(0.0f, 0.0f, 0.0f);
	if (depth <= (waterHeight - maxDepth)) {
		f.y = liquidDensity * volume;
	}
	else {
		float volumeFactor = (depth - maxDepth - waterHeight) / (2 * maxDepth);
		f.y = liquidDensity * volumeFactor * volume;
	}
	particle->addForce(f);
}

void ParticleBuoyancy::updateForce(RigidObject * particle, float t)
{
	if (particle->getCanMove()) {
		if (cube->getActivo()) {
			float depth = particle->getBody()->getGlobalPose().p.y;

			if (depth >= (waterHeight + maxDepth)) {
				return;
			}

			Vector3 f(0.0f, 0.0f, 0.0f);
			if (depth <= (waterHeight - maxDepth)) {
				f.y = liquidDensity * volume;
			}
			else {
				float volumeFactor = (depth - maxDepth - waterHeight) / (2 * maxDepth);
				f.y = liquidDensity * volumeFactor * volume;
			}
			particle->getBody()->addForce(f);
		}
	}
}


