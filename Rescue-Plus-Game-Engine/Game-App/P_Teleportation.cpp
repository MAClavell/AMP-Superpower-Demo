#include "P_Teleportation.h"

P_Teleportation::P_Teleportation()
{
}

P_Teleportation::~P_Teleportation()
{
}

// Activate behaviour
PowerPrevent P_Teleportation::Activate(short& currentJuice)
{
	return PowerPrevent::Nothing;
}

// Hold behaviour
PowerPrevent P_Teleportation::Hold(short& currentJuice)
{
	return PowerPrevent::Nothing;
}

// Stop behaviour
PowerPrevent P_Teleportation::Stop(short& currentJuice)
{
	return PowerPrevent::Nothing;
}

// Update the power, running activate, hold, and stop behaviour
PowerPrevent P_Teleportation::Update(PowerSlot slot, short& currentJuice)
{
	return PowerPrevent::Nothing;
}

// Call this function to move the character controller according to this power
bool P_Teleportation::MoveCharacter()
{
	return false;
}
