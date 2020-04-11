#pragma once
#include "GameObject.h"
#include "ColliderBase.h"
#include "RigidBody.h"

class Bolt : public UserComponent
{
private:
	DirectX::XMFLOAT3 velocity;
	ColliderBase* collider;
	RigidBody* rigidBody;
	CollisionLayers layersActive;
	CollisionLayers layersInActive;
	float timer;
	bool started;

public:
	Bolt(GameObject* gameObject);
	~Bolt();

	void Activate(GameObject* player);
	void Start();
	void Deactivate();

	void FixedUpdate() override;
	void Update() override;

	// --------------------------------------------------------
	// Override for a callback when a physics object collides with this
	// --------------------------------------------------------
	void OnCollisionEnter(Collision collision) override;
};

