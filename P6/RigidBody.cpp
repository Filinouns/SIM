#include "RigidBody.h"

RigidBody::RigidBody() {}

RigidBody::~RigidBody() {}

PxMat33 RigidBody::getWorldInverseInertiaTensor() const { 
	PxMat33 R(orientation); 
	PxMat33 invR = R.getTranspose(); 
	PxMat33 Iw = R * inverseInertiaTensor; 
	Iw = Iw * invR; 
	return Iw; 
}

void RigidBody::addForceAtPoint(const Vector3 & _f, const Vector3 & _p) {
	Vector3 pt = _p; 
	pt -= position;
	force += _f; 
	torque += pt.cross(_f);
}

void RigidBody::addForceAtLocalPoint(const Vector3 & _f, const Vector3 & _p) {
	Vector3 pt = transform.transform(_p); 
	addForceAtPoint(_f, pt);
}
