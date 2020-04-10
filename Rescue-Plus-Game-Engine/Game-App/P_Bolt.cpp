#include "P_Bolt.h"
#include "Times.h"

#define CHARGEUP 0.15f
#define COOLDOWN 0.5f

P_Bolt::P_Bolt()
{
	inputManager = InputManager::GetInstance();
	prevent = PowerPrevent::Nothing;
	timer = 0;

	//Create the bolt object
	bolt = 
}

P_Bolt::~P_Bolt()
{
}

PowerPrevent P_Bolt::Activate(Player& player, short& currentJuice)
{
	timer = CHARGEUP;
	active = true;

	//Enable bolt object

	return PowerPrevent::Nothing;
}

PowerPrevent P_Bolt::Hold(Player& player, short& currentJuice)
{
	timer -= Time::deltaTime();

	//Apply velocity to the bolt
	if (timer < 0)
	{
		shot = true;
	}

	return PowerPrevent::Nothing;
}

PowerPrevent P_Bolt::Stop(Player& player, short& currentJuice)
{
	active = false;

	//Disable bolt object

	return PowerPrevent::Nothing;
}

PowerPrevent P_Bolt::Update(Player& player, short& currentJuice)
{
	prevent = PowerPrevent::Nothing;

	//Chargeup
	if (active && !shot)
	{
		prevent = Hold(player, currentJuice);
	}
	//Cooldown and one bolt limit
	else if (timer > 0)
	{
		timer -= Time::deltaTime();
		if (timer <= 0)
		{
			prevent = Stop(player, currentJuice);
		}
	}
	//Activate
	else if (!active && inputManager->GetMouseButtonDown(MouseButtons::L))
	{
		prevent = Activate(player, currentJuice);
	}
	
	return prevent;
}
