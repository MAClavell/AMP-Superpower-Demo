#pragma once
#include "GameObject.h"
#include "RigidBody.h"

// --------------------------------------------------------
// Target dummies are 
// --------------------------------------------------------
class TargetDummy :
	public UserComponent
{
private:
	DirectX::XMFLOAT3 initialPosition;
	RigidBody* rb;
	float floorHeight;
	short health;

	// --------------------------------------------------------
	// Reset the target dummy to its initial position and health
	// --------------------------------------------------------
	void Reset();

public:
	// --------------------------------------------------------
	// Factory method for creating a Target Dummy object
	// --------------------------------------------------------
	static TargetDummy* TargetDummyFactory(const char* name,
		DirectX::XMFLOAT3 initialPosition, float floorHeight);

	TargetDummy(GameObject* gameObject, DirectX::XMFLOAT3 initialPosition,
		float floorHeight);
	~TargetDummy() { };

	void Update() override;

	// --------------------------------------------------------
	// Apply damage to the target dummy
	// --------------------------------------------------------
	void ApplyDamage(short damage);

	// --------------------------------------------------------
	// Apply a force to the target dummy
	// --------------------------------------------------------
	void AddForce(DirectX::XMFLOAT3 force);
};

