#include "Bolt.h"
#include "Collider.h"
#include "TargetDummy.h"
#include "ResourceManager.h"
#include "MeshRenderer.h"
#include "Times.h"

using namespace DirectX;

#define SPEED 3000
#define MAX_BOLT_TIME 3
#define BASE_BOLT_DAMAGE 1
#define MAX_BOLT_DAMAGE 10

Bolt::Bolt(GameObject* gameObject) : UserComponent(gameObject)
{
	started = false;
	timer = 0;
	layersActive = CollisionLayers(true);
	layersActive.Unset(CollisionLayer::Player);
	layersInActive = CollisionLayers(false);
	velocity = XMFLOAT3(0, 0, 0);
	damage = BASE_BOLT_DAMAGE;

	gameObject->AddComponent<MeshRenderer>(
		ResourceManager::GetInstance()->GetMesh("Assets\\Models\\Basic\\sphere.obj"),
		ResourceManager::GetInstance()->GetMaterial("white"));
	rigidBody = gameObject->AddComponent<RigidBody>(1);
	rigidBody->SetKinematic(true);
	collider = gameObject->AddComponent<SphereCollider>(1);
	collider->SetCollisionLayers(layersInActive);
}

Bolt::~Bolt()
{ }

void Bolt::Activate(GameObject* playerCamera)
{
	gameObject()->SetParent(playerCamera);
	gameObject()->SetEnabled(true);
	gameObject()->SetLocalPosition(0, -0.6f, 0.5f);
	gameObject()->SetLocalRotation(0, 0, 0);
}

void Bolt::Start(short damage)
{
	gameObject()->SetParent(nullptr);
	collider->SetCollisionLayers(layersActive);

	//Calculate velocity
	rigidBody->SetKinematic(false);
	XMStoreFloat3(&velocity, XMVectorScale(
		XMLoadFloat3(&gameObject()->GetForwardAxis()),
		SPEED * Time::fixedDeltaTime()));
	rigidBody->SetLinearVelocity(velocity);
	
	started = true;
	timer = MAX_BOLT_TIME;

	if (damage < BASE_BOLT_DAMAGE)
		damage = BASE_BOLT_DAMAGE;
	else if (damage > MAX_BOLT_DAMAGE)
		damage = MAX_BOLT_DAMAGE;
	this->damage = damage;
}

void Bolt::Deactivate()
{
	started = false;
	collider->SetCollisionLayers(layersInActive);
	rigidBody->SetLinearVelocity(0, 0, 0);
	rigidBody->SetKinematic(true);
	gameObject()->SetEnabled(false);
}

void Bolt::FixedUpdate()
{
	if (started)
	{
		rigidBody->SetLinearVelocity(velocity);
	}
}

void Bolt::Update()
{
	if (started)
	{
		//Despawn after some time
		if (timer < 0)
		{
			Deactivate();
		}
		else timer -= Time::deltaTime();
	}
}

void Bolt::OnCollisionEnter(Collision collision)
{
	Deactivate();

	if (collision.gameObject->GetName() == "TargetDummy")
	{
		collision.gameObject->GetComponent<TargetDummy>()->ApplyDamage(damage);
	}
}