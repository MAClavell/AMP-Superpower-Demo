#include "T_ChargedBlast.h"
#include "Times.h"
#include "Player.h"

#define CHARGEUP 0.3f
#define MAX_LEVEL 6;
#define DAMAGE_PER_LEVEL 1
#define TIME_PER_LEVEL 1.5f
#define JUICE_PER_LEVEL 1

T_ChargedBlast::T_ChargedBlast(Bolt* bolt)
{
	this->bolt = bolt;
	level = 1;
	timer = 0;
	chargeTimer = 0;
	inputManager = InputManager::GetInstance();
}

T_ChargedBlast::~T_ChargedBlast()
{ }

// Activate behaviour
PowerPrevent T_ChargedBlast::Activate(Player& player, short& currentJuice)
{
	timer = CHARGEUP;
	chargeTimer = 0;
	level = 1;
	bolt->Activate(player.GetCamera()->gameObject());
	return PowerPrevent::Nothing;
}

// Hold behaviour
PowerPrevent T_ChargedBlast::Hold(Player& player, short& currentJuice)
{
	timer -= Time::deltaTime();

	//Initial chargeup and then the user can hold to "level up" the bolt
	if (timer < 0)
	{
		if (level < 8)
		{
			chargeTimer += Time::deltaTime();
			if (chargeTimer > TIME_PER_LEVEL);
			{
				//Don't level up if no juice
				if (currentJuice < (level * JUICE_PER_LEVEL))
				{
					return PowerPrevent::Nothing;
				}
				chargeTimer -= TIME_PER_LEVEL;
				level++;
			}
		}
	}

	return PowerPrevent::Nothing;
}

// Stop behaviour
PowerPrevent T_ChargedBlast::Stop(Player& player, short& currentJuice)
{
	//Level 1 requires no extra juice cost
	if(level != 1)
		currentJuice -= level * JUICE_PER_LEVEL;
	bolt->Start(level * DAMAGE_PER_LEVEL);
	return PowerPrevent::Nothing;
}

// Update the power, running activate, hold, and stop behaviour
PowerPrevent T_ChargedBlast::Update(Player& player, short& currentJuice)
{
	PowerPrevent prevent = PowerPrevent::Nothing;

	if (inputManager->GetMouseButton(MouseButtons::R))
	{
		prevent = Hold(player, currentJuice);
	}
	else if (inputManager->GetMouseButtonUp(MouseButtons::R))
	{
		prevent = Stop(player, currentJuice);
	}

	return prevent;
}