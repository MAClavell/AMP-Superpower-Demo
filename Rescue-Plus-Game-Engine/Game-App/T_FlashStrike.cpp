#include "T_FlashStrike.h"
#include "Player.h"

using namespace DirectX;

#define MAX_ATTACK_TIME 2

T_FlashStrike::T_FlashStrike()
{
	teleportPosition = XMFLOAT3(0, 0, 0);
	active = false;
	teleport = false;
	timer = 0;
}

// Activate behaviour
PowerPrevent T_FlashStrike::Activate(Player& player, short& currentJuice)
{
	originalPosition = player.gameObject()->GetPosition();
	active = true;
	teleport = true;
	timer = 0;
	return PowerPrevent::Movement;
}

// Hold behaviour
PowerPrevent T_FlashStrike::Hold(Player& player, short& currentJuice)
{
	timer += deltaTime;
	return PowerPrevent::Movement;
}

// Stop behaviour
PowerPrevent T_FlashStrike::Stop(Player& player, short& currentJuice)
{
	teleportPosition = originalPosition;
	active = false;
	teleport = true;
	return PowerPrevent::Nothing;
}

// Update the power, running activate, hold, and stop behaviour
PowerPrevent T_FlashStrike::Update(Player& player, short& currentJuice)
{
	PowerPrevent prevent = PowerPrevent::Movement;

	if (timer < MAX_ATTACK_TIME)
	{
		prevent = Hold(player, currentJuice);
	}
	else
	{
		prevent = Stop(player, currentJuice);
	}

	return prevent;
}

// Call this function to move the character controller according to this power
bool T_FlashStrike::MoveCharacter(Player& player)
{
	if (teleport)
	{
		player.gameObject()->SetPosition(teleportPosition);
		teleport = false;
	}
	return false;
}
