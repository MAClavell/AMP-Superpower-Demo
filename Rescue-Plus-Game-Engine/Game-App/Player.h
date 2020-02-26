#pragma once
#include "GameObject.h"
#include "FirstPersonMovement.h"
#include "Power.h"

class Player :
	public UserComponent
{
private:
	FirstPersonMovement* fpm;

	Power* primary;
	Power* secondary;

	short maxJuice;
	short currentJuice;

public:
	// --------------------------------------------------------
	// Factory function to safely create a Player object
	//
	// name - the name of the root game object
	// --------------------------------------------------------
	static Player* PlayerFactory(const char* name, int screenWidth, int screenHeight);

	Player(GameObject* gameObject);
	~Player() { };

	void FixedUpdate(float fixedTimestep) override;
	void Update(float deltaTime) override;

	// --------------------------------------------------------
	// Get the camera attached to this player
	// --------------------------------------------------------
	Camera* GetCamera();
};

