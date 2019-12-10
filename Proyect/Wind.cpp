#include "Wind.h"



Wind::Wind(const Vector3& vient, Vector3 posicion, int r): myPos(posicion), trans(myPos)
{
	viento = vient;
	sphRadio = r;
	myColor = {0, 0, 0, 1};
	sphera = new RenderItem(CreateShape(*new PxSphereGeometry(sphRadio)), myColor);
	sphera->color = Vector4(1.0, 1.0, 0.0, 0.0);
	sphera->transform = &trans;
	
	RegisterRenderItem(sphera);
	sphera->addReference();
}

Wind::~Wind() {
	delete sphera;
	sphera = nullptr;
}

void Wind::updateForce(Particula* particle, float t) {
	if (!particle->hasFiniteMass()) {
		return;
	}
	//Just check if the distance from the point P to the center of the circle C is less than the radius of the circle.
	int dx = abs(particle->getPos().x - trans.p.x);
	int dy = abs(particle->getPos().y - trans.p.y);
	int dz = abs(particle->getPos().z - trans.p.z);
	if (dx + dy + dz < getRadio()) {
		particle->addForce((viento*particle->getMass())*360);
	}
}

void Wind::updateForce(RigidObject* particle, float t) {
	//Just check if the distance from the point P to the center of the circle C is less than the radius of the circle.
	/*int dx = abs(particle->getPos().x - trans.p.x);
	int dy = abs(particle->getPos().y - trans.p.y);
	int dz = abs(particle->getPos().z - trans.p.z);*/
	if (particle->getCanMove()) {
		Vector3 distancia = (particle->getBody()->getGlobalPose().p - trans.p);
		if (distancia.magnitude() < getRadio() && sphera->getActivo()) {
			particle->getBody()->addForce((viento*particle->getBody()->getMass()) * 360);
		}
	}
}
