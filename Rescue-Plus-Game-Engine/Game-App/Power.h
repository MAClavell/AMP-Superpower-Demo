#pragma once
#include <vector>

enum class PowerSlot { Primary, Secondary, Tertiary };
enum class PowerPrevent { Nothing, Powers, Movement };

class ITrick
{

};

// --------------------------------------------------------
// Interface for an enhancement. Inherit and implement all functions
// --------------------------------------------------------
__interface IEnhancement
{
	// --------------------------------------------------------
	// Update this enhancement
	// --------------------------------------------------------
	virtual void UpdateEnhancement() = 0;
};

// --------------------------------------------------------
// Abstract class for a power. Inherit and implement all functions
// --------------------------------------------------------
class Power
{
protected:
	bool canUse = true;
	std::vector<ITrick> tricks;
	std::vector<IEnhancement> enhancements;

	// --------------------------------------------------------
	// Activate behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	virtual PowerPrevent Activate() = 0;

	// --------------------------------------------------------
	// Hold behaviour
	// 
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	virtual PowerPrevent Hold() = 0;

	// --------------------------------------------------------
	// Stop behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	virtual PowerPrevent Stop() = 0;

	Power() { };
	~Power() { };

public:
	// --------------------------------------------------------
	// Update the power, running activate, hold, and stop behaviour
	//
	// return PowerPrevent - What the character should do after this power runs
	// --------------------------------------------------------
	virtual PowerPrevent UpdatePower(PowerSlot slot) = 0;

	// --------------------------------------------------------
	// Call this function to move the character controller according to this power
	//
	// return bool - If the power moved the character
	// --------------------------------------------------------
	virtual bool MoveCharacter() = 0;

	// --------------------------------------------------------
	// Get if this power can be used
	// --------------------------------------------------------
	virtual bool GetCanUse()
	{
		return canUse;
	};
};