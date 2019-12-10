#include "ParticleSpring.h"



ParticleSpring::ParticleSpring(float k_, float restLength_, Vector3* anchor_, Particula* other_): anchor(anchor_), trans(*anchor)
{
	other = other_;
	k = k_;
	restLength = restLength_;
	myColor = { 0, 0, 0, 1 };
	cube = new RenderItem(CreateShape(*new PxBoxGeometry(2, 2, 2)), myColor);
	cube->color = Vector4(1.0, 0.0, 0.0, 1.0);
	cube->transform = &trans;
	RegisterRenderItem(cube);
	cube->addReference();
}

ParticleSpring::ParticleSpring(float k_, float restLength_, Vector3* anchor_, RigidObject* other_) : anchor(anchor_), trans(*anchor)
{
	otherRigid = other_;
	k = k_;
	restLength = restLength_;
	myColor = { 0, 0, 0, 1 };
	cube = new RenderItem(CreateShape(*new PxBoxGeometry(2, 2, 2)), myColor);
	cube->color = Vector4(1.0, 0.0, 0.0, 1.0);
	cube->transform = &trans;
	RegisterRenderItem(cube);
	cube->addReference();
}

ParticleSpring::~ParticleSpring()
{
	cube->release();
	delete cube;
	cube = nullptr;
}

void ParticleSpring::updateForce(Particula* p, float t) {
	Vector3 f = p->getPos();
	if (other != nullptr) {
		f -= other->getPos();
	}
	else {
		f -= *anchor;
	}

	/*float magnitude = f.magnitude();
	magnitude = abs(magnitude - restLength);
	magnitude *= k;

	f.normalize();
	f *= -magnitude;
	p->addForce(f);*/


	//GOMA

	/*float length = f.normalize();

	length = (length - restLength);
	if (length <= 0.0f)
		return;

	f *= -length * k;
	p->addForce(f);*/


    float length = f.normalize();

	f *= -((length - restLength)*k);
	p->addForce(f);
}

void ParticleSpring::updateForce(RigidObject* p, float t) {
	if (p->getCanMove()) {
		if (cube->getActivo()) {
			Vector3 f = p->getBody()->getGlobalPose().p;
			if (otherRigid != nullptr) {
				f -= otherRigid->getBody()->getGlobalPose().p;
			}
			else {
				f -= *anchor;
			}

			/*float magnitude = f.magnitude();
			magnitude = abs(magnitude - restLength);
			magnitude *= k;

			f.normalize();
			f *= -magnitude;
			p->addForce(f);*/


			//GOMA

			/*float length = f.normalize();

			length = (length - restLength);
			if (length <= 0.0f)
				return;

			f *= -length * k;
			p->addForce(f);*/


			float length = f.normalize();

			f *= -((length - restLength)*k);
			p->getBody()->addForce(f);
		}
	}
}
