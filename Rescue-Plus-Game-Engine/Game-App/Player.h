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
	Power* equippedPower;

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
	~Player();

	void FixedUpdate() override;
	void Update() override;

	// --------------------------------------------------------
	// Get the camera attached to this player
	// --------------------------------------------------------
	Camera* GetCamera();

	// --------------------------------------------------------
	// Get the collider base attached to this player
	// --------------------------------------------------------
	ColliderBase* GetColliderBase();

	// --------------------------------------------------------
	// Get the current height of the player
	// --------------------------------------------------------
	float GetHeight();
};

