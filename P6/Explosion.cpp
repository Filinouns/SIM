#include "Explosion.h"

Explosion::Explosion(float f, Vector3 p, int r) : pos_(p), trans_(pos_), r_(r) {
	explosion_ = f;
	color_ = { 1, 0.92, 0.016, 0 };
	sphere_ = new RenderItem(CreateShape(*new PxSphereGeometry(r_)), &trans_, color_);

	cd_ = 20.0f;
	timePassed_ = 0;
	timeWorking_ = 0;
	active_ = false;

	RegisterRenderItem(sphere_);
}

Explosion::~Explosion() {
	delete sphere_;
	sphere_ = nullptr;
}

void Explosion::updateForce(Particle * p, double t) {
	if (!active_) return;
	if (!p->hasInfiniteMass()) return;

	
	timePassed_ += t;
	if (timePassed_ > cd_) {
		timePassed_ = 0;
		active_ = false;
		return;
	}
	
	// Comprobar la pos de la particula con el radio de accion
	Vector3 m = p->getPos() - pos_;
	if (m.x < r_ && m.y < r_ && m.z < r_) {
		//Vector3 m = p->getPos() - pos_;
		m = (m / m.magnitude() * explosion_) / m.magnitude();
		p->addForce(m*p->getMass());
	}
}