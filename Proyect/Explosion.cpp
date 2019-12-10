#include "Explosion.h"



Explosion::Explosion(float fuerza, float timeToDisappear, Vector3 pos, int r): myPos(pos), trans(myPos), timeExpl(timeToDisappear)
{
	myFuerza = fuerza;
	activo = false;
	puedoBorrar = false;
	sphRadio = r;
	secondsPassed = 0;
	nextTime = 0;
	myColor = { 0, 0, 0, 1 };
	sphera = new RenderItem(CreateShape(*new PxSphereGeometry(sphRadio)), myColor);
	sphera->color = Vector4(1.0, 0.0, 0.0, 0.0);
	sphera->transform = &trans;
	RegisterRenderItem(sphera);
	sphera->addReference();
}


Explosion::~Explosion()
{
	delete sphera;
	sphera = nullptr;
}

void Explosion::updateForce(Particula* p, float t) {
	/*float dx = abs(p->getPos().x - trans.p.x);
	float dy = abs(p->getPos().y - trans.p.y);
	float dz = abs(p->getPos().z - trans.p.z);
	Vector3 distancia(dx, dy, dz);*/
	Vector3 distancia = (p->getPos() - trans.p);

	if (activo) {
		secondsPassed += t;
		if (nextTime == 0)nextTime = secondsPassed + timeExpl;
		if (secondsPassed > nextTime) {
			activo = false;
			puedoBorrar = true;
			nextTime = secondsPassed + timeExpl;
		}
	/*if (distancia.magnitude() < getRadio() * 2) {
			p->addForce(distancia*myFuerza*p->getMass());
		}*/
		/*if((p->getPos() - pos))
		//Presuponiendo con radio 50*/
		if (distancia.magnitude() < getRadio() * 2) {
			//p->addForce(distancia.getNormalized() * myFuerza * (1 - (distancia.magnitude() / getRadio())) / distancia.magnitude());
			p->addForce(((distancia / distancia.magnitude()*myFuerza) / distancia.magnitude())*p->getMass());
			//p->addForce(distancia*myFuerza*p->getMass());
		}
	}
}
void Explosion::updateForce(RigidObject* particle, float t) {
	/*float dx = abs(p->getPos().x - trans.p.x);
	float dy = abs(p->getPos().y - trans.p.y);
	float dz = abs(p->getPos().z - trans.p.z);
	Vector3 distancia(dx, dy, dz);*/
	if (particle->getCanMove()) {
		Vector3 distancia = (particle->getBody()->getGlobalPose().p - trans.p);
		if (sphera->getActivo()) {
			if (activo) {
				secondsPassed += t;
				if (nextTime == 0)nextTime = secondsPassed + timeExpl;
				if (secondsPassed > nextTime) {
					activo = false;
					puedoBorrar = true;
					nextTime = secondsPassed + timeExpl;
				}
				/*if (distancia.magnitude() < getRadio() * 2) {
						p->addForce(distancia*myFuerza*p->getMass());
					}*/
					/*if((p->getPos() - pos))
					//Presuponiendo con radio 50*/
				if (distancia.magnitude() < getRadio()) {
					//p->addForce(distancia.getNormalized() * myFuerza * (1 - (distancia.magnitude() / getRadio())) / distancia.magnitude());
					//p->getBody()->addForce(((distancia / distancia.magnitude()*myFuerza) / distancia.magnitude())*p->getBody()->getMass());
					particle->getBody()->addForce(distancia*myFuerza*particle->getBody()->getMass());
				}
			}
		}
	}
}