#include "T_FlashStrike.h"
#include "Player.h"
#include "Times.h"
#include "TargetDummy.h"

using namespace DirectX;

#define MAX_ATTACK_TIME 1
#define DAMAGE 2

T_FlashStrike::T_FlashStrike()
{
	teleportPosition = XMFLOAT3(0, 0, 0);
	teleportRotation = XMFLOAT4(0, 0, 0, 0);
	originalPosition = XMFLOAT3(0, 0, 0);
	originalRotation = XMFLOAT4(0, 0, 0, 0);
	active = false;
	teleport = false;
	timer = 0;
}

// Send teleport positioning data to the flash strike
void T_FlashStrike::SendTeleportData(DirectX::XMFLOAT3 teleportToPosition)
{
	teleportPosition = teleportToPosition;
}

// Apply damage to the enemy target of the flash strike
void T_FlashStrike::ApplyDamage(GameObject& enemy)
{
	TargetDummy* td = enemy.GetComponent<TargetDummy>();
	td->ApplyDamage(DAMAGE);
}

// Activate behaviour
PowerPrevent T_FlashStrike::Activate(Player& player, short& currentJuice)
{
	teleportRotation = player.gameObject()->GetRotation();
	originalPosition = player.gameObject()->GetPosition();
	originalRotation = player.gameObject()->GetRotation();
	active = true;
	teleport = true;
	timer = 0;
	return PowerPrevent::MovementAndCamera;
}

// Hold behaviour
PowerPrevent T_FlashStrike::Hold(Player& player, short& currentJuice)
{
	timer += Time::deltaTime();
	return PowerPrevent::MovementAndCamera;
}

// Stop behaviour
PowerPrevent T_FlashStrike::Stop(Player& player, short& currentJuice)
{
	teleportPosition = originalPosition;
	teleportRotation = originalRotation;
	active = false;
	teleport = true;
	return PowerPrevent::Nothing;
}

// Update the power, running activate, hold, and stop behaviour
PowerPrevent T_FlashStrike::Update(Player& player, short& currentJuice)
{
	PowerPrevent prevent = PowerPrevent::MovementAndCamera;

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
		player.gameObject()->SetRotation(teleportRotation);
		teleport = false;
	}
	return false;
}
