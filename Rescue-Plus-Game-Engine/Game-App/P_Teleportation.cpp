#include "P_Teleportation.h"
#include "Player.h"
#include "Renderer.h"
#include "PhysicsManager.h"

using namespace DirectX;

P_Teleportation::P_Teleportation()
{
	teleportPosition = XMFLOAT3(0, 0, 0);
	inputManager = InputManager::GetInstance();
	prevent = PowerPrevent::Nothing;
	maxRange = 30;
	active = false;
	teleport = false;
	layers = CollisionLayers(true);
	layers.Unset(CollisionLayer::Player);
}

P_Teleportation::~P_Teleportation()
{ }

// Activate behaviour
PowerPrevent P_Teleportation::Activate(Player& player, short& currentJuice)
{
	active = true;
	return PowerPrevent::Nothing;
}

// Hold behaviour
PowerPrevent P_Teleportation::Hold(Player& player, short& currentJuice)
{
	GameObject* cameraGO = player.GetCamera()->gameObject();

	//Have to find a valid position
	//Repeatedly raycast to see if we have a valid position
	float rangeMod = 0;
	float halfHeight = player.GetHeight() / 2;
	while (true)
	{
		RaycastHit hit;
		if (Raycast(cameraGO->GetPosition(),
			cameraGO->GetForwardAxis(), &hit, maxRange - rangeMod,
			layers, ShapeDrawType::ForDuration, 0.5f))
		{
			teleportPosition = hit.point;
		}
		else XMStoreFloat3(&teleportPosition, XMVectorAdd(XMLoadFloat3(&cameraGO->GetPosition()),
			XMVectorScale(XMLoadFloat3(&cameraGO->GetForwardAxis()),
				maxRange - rangeMod)));

		//Check if we can fit at the location
		SweepHit swHit;
		if (!Sweep(player.GetColliderBase(), teleportPosition, player.gameObject()->GetForwardAxis(), &swHit, 0,
			layers))
		{
			break;
		}

		//Check if we can fit above the point
		XMFLOAT3 tempPos = teleportPosition;
		tempPos.y += halfHeight;
		if (!Sweep(player.GetColliderBase(), tempPos, player.gameObject()->GetUpAxis(), &swHit, 0,
			layers))
		{
			teleportPosition = tempPos;
			break;
		}

		//Check if we can fit below the point
		tempPos = teleportPosition;
		tempPos.y -= halfHeight;
		if (!Sweep(player.GetColliderBase(), tempPos, player.gameObject()->GetUpAxis(), &swHit, 0,
			layers))
		{
			teleportPosition = tempPos;
			break;
		}

		//Move the check backwards if no valid position was found
		rangeMod += 0.25f;
	}

	//Move the teleporter indicator
	XMFLOAT4 rot;
	XMStoreFloat4(&rot, XMQuaternionMultiply(XMQuaternionRotationAxis(XMVectorSet(0, 0, 1, 0), physx::PxHalfPi),
		XMLoadFloat4(&player.gameObject()->GetRotation())));
	Renderer::GetInstance()->AddDebugCapsule(1, halfHeight * 2, teleportPosition, rot, ShapeDrawType::SingleFrame);

	return PowerPrevent::Nothing;
}

// Stop behaviour
PowerPrevent P_Teleportation::Stop(Player& player, short& currentJuice)
{
	active = false;
	teleport = true;
	return PowerPrevent::Nothing;
}

// Update the power, running activate, hold, and stop behaviour
PowerPrevent P_Teleportation::Update(Player& player, PowerSlot slot, short& currentJuice)
{
	prevent = PowerPrevent::Nothing;

	//Activate
	if (inputManager->GetMouseButtonDown(MouseButtons::R))
	{
		prevent = Activate(player, currentJuice);
	}
	//Deactivate
	else if (active && inputManager->GetMouseButtonUp(MouseButtons::R))
	{
		prevent = Stop(player, currentJuice);
	}
	//Holding button down
	else if (active)
	{
		prevent = Hold(player, currentJuice);
	}

	return prevent;
}

// Call this function to move the character controller according to this power
bool P_Teleportation::MoveCharacter(Player& player)
{
	if (teleport)
	{
		player.gameObject()->SetPosition(teleportPosition);
		teleport = false;
		return true;
	}
	return false;
}
