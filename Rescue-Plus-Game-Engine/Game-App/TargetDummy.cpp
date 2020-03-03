#include "TargetDummy.h"
#include "Collider.h"
#include "ResourceManager.h"
#include "MeshRenderer.h"

using namespace DirectX;

#define MAX_HEALTH 3

// Factory method for creating a Target Dummy object
TargetDummy* TargetDummy::TargetDummyFactory(const char* name,
	XMFLOAT3 initialPosition, float floorHeight)
{
	GameObject* root = new GameObject(name);
	root->AddComponent<MeshRenderer>(
		ResourceManager::GetInstance()->GetMesh("Assets\\Models\\Basic\\capsule.obj"),
		ResourceManager::GetInstance()->GetMaterial("orange"));
	root->AddComponent<RigidBody>(1.0f)->SetContraints(false, false, false, true, true, true);
	root->AddComponent<CapsuleCollider>(1.0f, 2.0f, CapsuleDirection::Y);
	return root->AddComponent<TargetDummy>(initialPosition, floorHeight);
}

TargetDummy::TargetDummy(GameObject* gameObject, XMFLOAT3 initialPosition,
	float floorHeight) : UserComponent(gameObject)
{
	this->initialPosition = initialPosition;
	gameObject->SetPosition(initialPosition);
	this->floorHeight = floorHeight;
	health = MAX_HEALTH;
	rb = gameObject->GetComponent<RigidBody>();
}

void TargetDummy::Update(float deltaTime)
{
	if (gameObject()->GetPosition().y < floorHeight)
		Reset();
}

// Reset the target dummy to its initial position and health
void TargetDummy::Reset()
{
	gameObject()->SetPosition(initialPosition);
	health = MAX_HEALTH;
	rb->SetLinearVelocity(XMFLOAT3(0, 0, 0));
}

// Apply damage to the target dummy
void TargetDummy::ApplyDamage(short damage)
{
	health -= damage;
	if (health <= 0)
		Reset();
}

// Apply a force to the target dummy
void TargetDummy::AddForce(DirectX::XMFLOAT3 force)
{
	rb->AddForce(force);
}
