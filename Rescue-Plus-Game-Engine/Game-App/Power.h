#pragma once
#include <vector>

enum class PowerSlot { Primary, Secondary, Tertiary };
enum class PowerPrevent { Nothing, Powers, Movement };

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

__interface IPower
{
	// --------------------------------------------------------
	// Activate behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	virtual PowerPrevent Activate(short& currentJuice) = 0;

	// --------------------------------------------------------
	// Hold behaviour
	// 
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	virtual PowerPrevent Hold(short& currentJuice) = 0;

	// --------------------------------------------------------
	// Stop behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	virtual PowerPrevent Stop(short& currentJuice) = 0;

	// --------------------------------------------------------
	// Update the power, running activate, hold, and stop behaviour
	//
	// return PowerPrevent - What the character should do after this power runs
	// --------------------------------------------------------
	virtual PowerPrevent Update(PowerSlot slot, short& currentJuice) = 0;

	// --------------------------------------------------------
	// Call this function to move the character controller according to this power or trick
	//
	// return bool - If the power moved the character
	// --------------------------------------------------------
	virtual bool MoveCharacter() = 0;
};

// --------------------------------------------------------
// Abstract class for a power. Inherit and implement all functions
// --------------------------------------------------------
class Power : public IPower
{
protected:
	bool canUse = true;
	std::vector<IPower> tricks;
	std::vector<IEnhancement> enhancements;

	Power() { };
	~Power() { };

public:

	// --------------------------------------------------------
	// Get if this power can be used
	// --------------------------------------------------------
	virtual bool GetCanUse()
	{
		return canUse;
	};
};