#include "P_Teleportation.h"
#include "Player.h"

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
	/*if (Sweep(player.GetColliderBase(), player.GetCamera()->gameObject()->GetForwardAxis(), &hit, 10,
		layers, ShapeDrawType::ForDuration, 10))
	{
		printf("Sweep hit on: %s\n", hit.gameObject->GetName().c_str());
	}*/

	GameObject* cameraGO = player.GetCamera()->gameObject();
	XMFLOAT3 pos = XMFLOAT3(0, 0, 0);
	
	//See if we hit any physics objects
	if (Raycast(cameraGO->GetPosition(),
		cameraGO->GetForwardAxis(), &hit, maxRange,
		layers, ShapeDrawType::ForDuration, 4))
	{
		pos = hit.point;
		printf("Raycast hit on: %s\n", hit.gameObject->GetName().c_str());
	}
	//We didn't, so calculate our reticle position
	else
	{
		//pos = (cameraGO->GetPosition() + cameraGO->GetForwardAxis()) * maxRange;
	}

	//Check if player can fit

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
		//player.gameObject()->SetPosition(teleportPosition);
		teleport = false;
		return true;
	}
	return false;
}
