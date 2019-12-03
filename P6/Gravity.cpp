#include "Gravity.h"

Gravity::Gravity(const Vector3& _g) {
}

Gravity::~Gravity() {}

void Gravity::updateForce(RigidBody* body, float t) {
	if (body->hasInfiniteMass()) return;
	body->addForce(gravity * body->getMass());
}