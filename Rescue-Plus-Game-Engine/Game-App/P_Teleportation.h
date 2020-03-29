#pragma once
#include "Power.h"
#include "InputManager.h"
#include "Raycast.h"

class P_Teleportation : public Power
{
	DirectX::XMFLOAT3 teleportPosition;
	InputManager* inputManager;
	PowerPrevent prevent;
	CollisionLayers layers;
	float maxRange;
	bool active;
	bool teleport;

private:

	// --------------------------------------------------------
	// Activate behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	PowerPrevent Activate(Player& player, short& currentJuice) override;

	// --------------------------------------------------------
	// Hold behaviour
	// 
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	PowerPrevent Hold(Player& player, short& currentJuice) override;

	// --------------------------------------------------------
	// Stop behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	PowerPrevent Stop(Player& player, short& currentJuice) override;

public:

	P_Teleportation();
	~P_Teleportation();

	// --------------------------------------------------------
	// Update the power, running activate, hold, and stop behaviour
	//
	// return PowerPrevent - What the character should do after this power runs
	// --------------------------------------------------------
	virtual PowerPrevent Update(Player& player, PowerSlot slot, short& currentJuice) override;

	// --------------------------------------------------------
	// Call this function to move the character controller according to this power
	//
	// return bool - If the power moved the character
	// --------------------------------------------------------
	virtual bool MoveCharacter(Player& player) override;
};

