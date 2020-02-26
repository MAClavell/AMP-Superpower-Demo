#pragma once
#include "Power.h"

class P_Teleportation : public Power
{
private:

	// --------------------------------------------------------
	// Activate behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	PowerPrevent Activate() override;

	// --------------------------------------------------------
	// Hold behaviour
	// 
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	PowerPrevent Hold() override;

	// --------------------------------------------------------
	// Stop behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	PowerPrevent Stop() override;

public:

	P_Teleportation();
	~P_Teleportation();

	// --------------------------------------------------------
	// Update the power, running activate, hold, and stop behaviour
	//
	// return PowerPrevent - What the character should do after this power runs
	// --------------------------------------------------------
	virtual PowerPrevent UpdatePower(PowerSlot slot) override;

	// --------------------------------------------------------
	// Call this function to move the character controller according to this power
	//
	// return bool - If the power moved the character
	// --------------------------------------------------------
	virtual bool MoveCharacter() override;
};

