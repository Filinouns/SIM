#pragma once

#include "core.hpp"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"

using namespace physx;

class RigidBody {
public:
	RigidBody();
	virtual ~RigidBody();

	void setInertiaTensor(const PxMat33& inertiaTensor) { inverseInertiaTensor = inertiaTensor.getInverse(); }
	PxMat33 getWorldInverseInertiaTensor() const;

	// Apply force in the center of mass 
	void addForce(const Vector3& _f) { force += _f; }
	// Apply force at point 
	void addForceAtPoint(const Vector3& _f, const Vector3& _p);
	void addForceAtLocalPoint(const Vector3& _f, const Vector3& _p);
	// clear forces and torques 
	void clearAccumulators() { force.isZero(); torque.isZero(); }

	//Masa
	bool hasInfiniteMass() const { return inverse_mass >= 0.0f; }
	int getMass() {
		if (inverse_mass <= 0) return 0;
		else return ((int)1) / inverse_mass;
	}

	Vector3 getWorldPosition() { return transform.p; }

protected: 
	float inverse_mass;
	float linearDamping;

	Vector3 position; 
	Vector4 orientation;
	// Velocities 
	Vector3 velocity; // linear 
	Vector3 rotation; // angular

	// From local to world 
	PxTransform transform;

	// Inverse tensor in local coordinates 
	PxMat33 inverseInertiaTensor;

	Vector3 force; 
	Vector3 torque;
};

