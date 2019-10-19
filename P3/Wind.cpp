#include "Wind.h"

Wind::Wind(const Vector3& v, Vector3 p, int r) : pos_(p), trans_(pos_), r_(r) {
	wind_ = v;
	color_ = { 0, 0, 0, 1 };
	sphere_ = new RenderItem(CreateShape(*new PxSphereGeometry(r_)), &trans_, color_);

	RegisterRenderItem(sphere_);
}


Wind::~Wind() {}

void Wind::updateForce(Particle * p, float t) {
	if (!p->hasInfiniteMass()) return;

	//Just check if the distance from the point P to the center of the circle C is less than the radius of the circle.
	int dx = abs(p->getPos().x - trans_.p.x);
	int dy = abs(p->getPos().y - trans_.p.y);
	int dz = abs(p->getPos().z - trans_.p.z);
	if (dx + dy + dz < getR()) {
		p->addForce(wind_*p->getMass());
	}
}
