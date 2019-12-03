#include "Wind.h"

Wind::Wind(const Vector3& v, Vector3 p, int r) : pos_(p), trans_(pos_), r_(r) {
	wind_ = v;
	color_ = { 1, 0, 0, 0 };
	sphere_ = new RenderItem(CreateShape(*new PxSphereGeometry(r_)), &trans_, color_);

	rendering = true;
	RegisterRenderItem(sphere_);
}

Wind::~Wind() {
	DeregisterRenderItem(sphere_);
	delete sphere_;
	sphere_ = nullptr;
}

void Wind::updateForce(Particle * p, double t) {
	if (!p->hasInfiniteMass()) return;

	// Comprobar la pos de la particula con el radio de accion
	int dx = abs(p->getPos().x - pos_.x);
	int dy = abs(p->getPos().y - pos_.y);
	int dz = abs(p->getPos().z - pos_.z);
	if (dx < r_ && dy < r_ && dz < r_) p->addForce(wind_*p->getMass());
}

void Wind::unrender() {
	if (rendering) {
		DeregisterRenderItem(sphere_);
		rendering = false;
	}
	else {
		RegisterRenderItem(sphere_);
		rendering = true;
	}
}