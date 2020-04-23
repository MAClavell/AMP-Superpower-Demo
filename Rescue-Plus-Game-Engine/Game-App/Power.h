#pragma once
#include <unordered_map>
#include <string>

enum class PowerPrevent { Nothing, Movement, MovementAndCamera };

class Player;

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
	virtual PowerPrevent Activate(Player& player, short& currentJuice) = 0;

	// --------------------------------------------------------
	// Hold behaviour
	// 
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	virtual PowerPrevent Hold(Player& player, short& currentJuice) = 0;

	// --------------------------------------------------------
	// Stop behaviour
	//
	// return PowerPrevent - What the character should do after this function returns
	// --------------------------------------------------------
	virtual PowerPrevent Stop(Player& player, short& currentJuice) = 0;

	// --------------------------------------------------------
	// Update the power, running activate, hold, and stop behaviour
	//
	// return PowerPrevent - What the character should do after this power runs
	// --------------------------------------------------------
	virtual PowerPrevent Update(Player& player, short& currentJuice) = 0;

	// --------------------------------------------------------
	// Call this function to move the character controller according to this power or trick
	//
	// return bool - If the power moved the character
	// --------------------------------------------------------
	virtual bool MoveCharacter(Player& player) = 0;
};

// --------------------------------------------------------
// Abstract class for a power. Inherit and implement all functions
// --------------------------------------------------------
class Power : public IPower
{
protected:
	bool canUse = true;
	std::unordered_map<std::string, IPower*> tricks;
	std::unordered_map<std::string, IEnhancement*> enhancements;
	std::string name;

	Power() { };

public:
	virtual ~Power() 
	{
		for (auto t : tricks)
		{
			if(t.second != nullptr)
				delete t.second;
		}
		for (auto e : enhancements)
		{
			if(e.second != nullptr)
				delete e.second;
		}
	};

	// --------------------------------------------------------
	// Get if this power can be used
	// --------------------------------------------------------
	virtual bool GetCanUse()
	{
		return canUse;
	};

	// --------------------------------------------------------
	// Get the name of this power
	// --------------------------------------------------------
	std::string GetName() { return name; }
};