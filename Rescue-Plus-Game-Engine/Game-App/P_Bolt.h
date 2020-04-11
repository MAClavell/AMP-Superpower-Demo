#pragma once
#include "Power.h"
#include "GameObject.h"
#include "InputManager.h"
#include "Bolt.h"

class P_Bolt : public Power
{
	Bolt* bolt;
	InputManager* inputManager;
	PowerPrevent prevent;
	bool active;
	bool shot;
	float timer;

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

	P_Bolt();
	~P_Bolt();

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
	virtual bool MoveCharacter(Player& player) override { return false; };
};

