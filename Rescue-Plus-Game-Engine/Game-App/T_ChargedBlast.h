#pragma once
#include "Power.h"
#include "Bolt.h"
#include "InputManager.h"

class T_ChargedBlast : public IPower
{
private:
	Bolt* bolt;
	short level;
	float timer;
	float chargeTimer;
	InputManager* inputManager;

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

	// --------------------------------------------------------
	// Call this function to move the character controller according to this power
	//
	// return bool - If the power moved the character
	// --------------------------------------------------------
	virtual bool MoveCharacter(Player& player) override { return false; };

public:

	T_ChargedBlast(Bolt* bolt);
	~T_ChargedBlast();

	// --------------------------------------------------------
	// Activate behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	PowerPrevent Activate(Player& player, short& currentJuice) override;

	// --------------------------------------------------------
	// Update the power, running activate, hold, and stop behaviour
	//
	// return PowerPrevent - What the character should do after this power runs
	// --------------------------------------------------------
	virtual PowerPrevent Update(Player& player, short& currentJuice) override;
};

