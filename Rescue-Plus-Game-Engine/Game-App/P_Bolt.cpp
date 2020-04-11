#include "P_Bolt.h"
#include "Times.h"
#include "Player.h"

#define CHARGEUP 0.3f
#define COOLDOWN 0.75f

P_Bolt::P_Bolt()
{
	inputManager = InputManager::GetInstance();
	prevent = PowerPrevent::Nothing;
	timer = 0;
	active = false;
	shot = false;

	//Create the bolt object
	GameObject* boltObj = new GameObject("Bolt");
	bolt = boltObj->AddComponent<Bolt>();
	boltObj->SetEnabled(false);
}

P_Bolt::~P_Bolt()
{ }

PowerPrevent P_Bolt::Activate(Player& player, short& currentJuice)
{
	timer = CHARGEUP;
	active = true;
	shot = false;

	//Enable bolt object
	bolt->Activate(player.GetCamera()->gameObject());

	return PowerPrevent::Nothing;
}

PowerPrevent P_Bolt::Hold(Player& player, short& currentJuice)
{
	timer -= Time::deltaTime();

	//Apply velocity to the bolt
	if (timer < 0)
	{
		shot = true;
		timer = COOLDOWN;
		bolt->Start();
	}

	return PowerPrevent::Nothing;
}

PowerPrevent P_Bolt::Stop(Player& player, short& currentJuice)
{
	active = false;
	shot = false;

	return PowerPrevent::Nothing;
}

PowerPrevent P_Bolt::Update(Player& player, short& currentJuice)
{
	prevent = PowerPrevent::Nothing;

	//Chargeup
	if (active && shot && !bolt->gameObject()->GetEnabled())
	{
		prevent = Stop(player, currentJuice);
	}
	else if (active && !shot)
	{
		prevent = Hold(player, currentJuice);
	}
	//Cooldown and one bolt limit
	else if (timer > 0)
	{
		timer -= Time::deltaTime();
	}
	//Activate
	else if (!active && inputManager->GetMouseButtonDown(MouseButtons::L))
	{
		prevent = Activate(player, currentJuice);
	}
	
	return prevent;
}
