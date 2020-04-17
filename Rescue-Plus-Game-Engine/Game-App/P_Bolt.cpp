#include "P_Bolt.h"
#include "Times.h"
#include "Player.h"
#include "T_ChargedBlast.h"

#define CHARGEUP 0.3f
#define COOLDOWN 0.75f

P_Bolt::P_Bolt()
{
	inputManager = InputManager::GetInstance();
	prevent = PowerPrevent::Nothing;
	timer = 0;
	active = false;
	shot = false;
	chargedBlastActive = false;

	//Create the bolt object
	GameObject* boltObj = new GameObject("Bolt");
	bolt = boltObj->AddComponent<Bolt>();
	boltObj->SetEnabled(false);

	//Add tricks
	tricks.insert({ "ChargedBlast", new T_ChargedBlast(bolt) });
}

P_Bolt::~P_Bolt()
{ }

// Activate behaviour
PowerPrevent P_Bolt::Activate(Player& player, short& currentJuice)
{
	timer = CHARGEUP;
	active = true;
	shot = false;

	//Enable bolt object
	bolt->Activate(player.GetCamera()->gameObject());

	return PowerPrevent::Nothing;
}

// Hold behaviour
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

// Stop behaviour
PowerPrevent P_Bolt::Stop(Player& player, short& currentJuice)
{
	active = false;
	shot = false;

	return PowerPrevent::Nothing;
}

// Update the power, running activate, hold, and stop behaviour
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
	else if (chargedBlastActive)
	{
		prevent = ((T_ChargedBlast*)tricks["ChargedBlast"])->Update(player, currentJuice);
		if (!bolt->gameObject()->GetEnabled())
		{
			prevent = Stop(player, currentJuice);
			chargedBlastActive = false;
		}
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
	else if (!chargedBlastActive && inputManager->GetMouseButtonDown(MouseButtons::R))
	{
		prevent = ((T_ChargedBlast*)tricks["ChargedBlast"])->Activate(player, currentJuice);
		chargedBlastActive = true;
		timer = COOLDOWN;
	}
	
	return prevent;
}
