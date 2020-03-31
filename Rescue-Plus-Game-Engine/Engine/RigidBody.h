#pragma once
#include <PxPhysicsAPI.h>
#include "GameObject.h"
#include "CollisionResolver.h"

// --------------------------------------------------------
// A rigid body definition.
//
// Allows a gameobject to be acted upon by the physics world
// --------------------------------------------------------
class RigidBody : public Component
{
private:
	physx::PxRigidDynamic* body;
	CollisionResolver* collisionResolver;

	// --------------------------------------------------------
	// Attach all children colliders to this GO
	// --------------------------------------------------------
	void FindChildrenColliders(GameObject*, bool firstObj);

	// --------------------------------------------------------
	// Update the rigidbody from it's world position and rotation
	// --------------------------------------------------------
	void UpdateRigidbodyWorld();

	// --------------------------------------------------------
	// Update the rigidbody from it's world position
	// --------------------------------------------------------
	void UpdateRigidbodyPosition(DirectX::XMFLOAT3 pos, bool fromParent, bool fromRigidBody);

	// --------------------------------------------------------
	// Update the rigidbody from it's world rotation
	// --------------------------------------------------------
	void UpdateRigidbodyRotation(DirectX::XMFLOAT4 rot, bool fromParent, bool fromRigidBody);

public:
	RigidBody(GameObject* gameObject, float mass);

	~RigidBody();

	// --------------------------------------------------------
	// Update collisions
	// --------------------------------------------------------
	void FixedUpdate() override;

	// --------------------------------------------------------
	// Update the gameobject's world position from an inputted transform
	// --------------------------------------------------------
	void UpdateWorldPosition();

	// --------------------------------------------------------
	// Set the mass of this rigid body
	// --------------------------------------------------------
	void SetMass(float mass);

	// --------------------------------------------------------
	// If an object is kinematic, no forces will act on it
	// --------------------------------------------------------
	void SetKinematic(bool kinematic);

	// --------------------------------------------------------
	// Get the mass of this rigid body (a mass of 0 is not dynamic)
	// --------------------------------------------------------
	float GetMass();

	// --------------------------------------------------------
	// Get the actual physx rigid body
	// --------------------------------------------------------
	physx::PxRigidDynamic* GetRigidBody();

	// --------------------------------------------------------
	// Add a force to this rigidbody
	//
	// force - the force vector to add
	// mode - the way to apply this force
	// --------------------------------------------------------
	void AddForce(DirectX::XMFLOAT3 force, physx::PxForceMode::Enum mode = physx::PxForceMode::eFORCE);

	// --------------------------------------------------------
	// Add a force to this rigidbody
	//
	// x - x component of the force
	// y - y component of the force
	// z - z component of the force
	// mode - the way to apply this force
	// --------------------------------------------------------
	void AddForce(float x, float y, float z, physx::PxForceMode::Enum mode = physx::PxForceMode::eFORCE);

	// --------------------------------------------------------
	// Get the current linear velocity for this rigidbody
	// --------------------------------------------------------
	DirectX::XMFLOAT3 GetLinearVelocity();

	// --------------------------------------------------------
	// Set the current linear velocity for this rigidbody
	// --------------------------------------------------------
	void SetLinearVelocity(DirectX::XMFLOAT3 velocity);

	// --------------------------------------------------------
	// Set the current linear velocity for this rigidbody
	// --------------------------------------------------------
	void SetLinearVelocity(float x, float y, float z);

	// --------------------------------------------------------
	// Get the maximum linear velocity for this rigidbody
	// --------------------------------------------------------
	float GetMaxLinearVelocity();

	// --------------------------------------------------------
	// Set the maximum linear velocity for this rigidbody
	// --------------------------------------------------------
	void SetMaxLinearVelocity(float max);

	// --------------------------------------------------------
	// Set the contraints of the rigidbody
	// Restricts movement or rotation on the axis
	// --------------------------------------------------------
	void SetContraints(bool lockPosX, bool lockPosY, bool lockPosZ,
		bool lockRotX, bool lockRotY, bool lockRotZ);

	// --------------------------------------------------------
	// WARNING: THIS IS FOR INTERNAL ENGINE USE ONLY. DO NOT USE
	// Get the collision resolver for this rigidbody.
	// --------------------------------------------------------
	CollisionResolver* GetCollisionResolver();
};

