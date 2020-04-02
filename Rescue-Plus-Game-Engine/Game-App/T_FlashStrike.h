#pragma once
#include "GameObject.h"
#include "Power.h"

class T_FlashStrike : public IPower
{
private:
	DirectX::XMFLOAT3 originalPosition;
	DirectX::XMFLOAT4 originalRotation;
	DirectX::XMFLOAT3 teleportPosition;
	DirectX::XMFLOAT4 teleportRotation;
	float timer;
	bool active;
	bool teleport;


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
	T_FlashStrike();

	// --------------------------------------------------------
	// Activate behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	PowerPrevent Activate(Player& player, short& currentJuice) override;

	// --------------------------------------------------------
	// Send teleport positioning data to the flash strike
	// --------------------------------------------------------
	void SendTeleportData(DirectX::XMFLOAT3 teleportToPosition);

	// --------------------------------------------------------
	// Apply damage to the enemy target of the flash strike
	// --------------------------------------------------------
	void ApplyDamage(GameObject& enemy);

	// --------------------------------------------------------
	// Update the power, running activate, hold, and stop behaviour
	//
	// return PowerPrevent - What the character should do after this power runs
	// --------------------------------------------------------
	virtual PowerPrevent Update(Player& player, short& currentJuice) override;

	// --------------------------------------------------------
	// Call this function to move the character controller according to this power
	//
	// return bool - If the power moved the character
	// --------------------------------------------------------
	virtual bool MoveCharacter(Player& player) override;
};

