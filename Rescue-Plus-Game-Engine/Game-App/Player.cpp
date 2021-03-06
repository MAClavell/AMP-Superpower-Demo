#include "Player.h"
#include "P_Teleportation.h"
#include "P_Bolt.h"

Player::Player(GameObject* gameObject) : UserComponent(gameObject)
{
	fpm = gameObject->GetComponent<FirstPersonMovement>();
	maxJuice = 8;
	currentJuice = maxJuice;
	inputManager = InputManager::GetInstance();

	//Setup powers
	primary = new P_Teleportation();
	secondary = new P_Bolt();
	equippedPower = primary;
}

Player::~Player() 
{
	if (primary != nullptr)
		delete primary;
	if (secondary != nullptr)
		delete secondary;
}


// Factory function to safely create a Player object
Player* Player::PlayerFactory(const char* name, int screenWidth, int screenHeight)
{
	//Make object
	GameObject* root = new GameObject(name);
	
	//Add first person movement
	FirstPersonMovement::FirstPersonMovementFactory(root, screenWidth, screenHeight);

	//Add player component
	Player* player = root->AddComponent<Player>();

	return player;
}

void Player::FixedUpdate()
{
	//Move the player from powers
	if (equippedPower != nullptr)
		equippedPower->MoveCharacter(*this);
}

void Player::Update()
{
	PowerPrevent prevent = PowerPrevent::Nothing;

	//Update all the powers
	if (equippedPower != nullptr)
		prevent = equippedPower->Update(*this, currentJuice);

	if (prevent == PowerPrevent::Movement)
	{
		fpm->SetControlsActive(false);
		fpm->SetCameraControlsActive(true);
	}
	else if (prevent == PowerPrevent::MovementAndCamera)
	{
		fpm->SetControlsActive(false);
		fpm->SetCameraControlsActive(false);
	}
	else
	{
		fpm->SetControlsActive(true);
		fpm->SetCameraControlsActive(true);

		if (inputManager->GetKeyDown(Key::One))
			equippedPower = primary;
		else if (inputManager->GetKeyDown(Key::Two))
			equippedPower = secondary;
	}

	//FOR DEMO ONLY
	//Add juice
	if (inputManager->GetKeyDown(Key::J))
	{
		currentJuice += currentJuice != maxJuice ? 1 : 0;
	}
}

// Get the camera attached to this player
Camera* Player::GetCamera()
{
	return fpm->GetCamera();
}

// Get the collider base attached to this player
ColliderBase* Player::GetColliderBase()
{
	return fpm->GetColliderBase();
}

// Get the current height of the player
float Player::GetHeight()
{
	return fpm->GetHeight();
}

// Get the name of the equipped power
std::string Player::GetEquippedPowerName()
{
	return equippedPower->GetName();
}

// Get the current amount of juice
short Player::GetCurrentJuice()
{
	return currentJuice;
}

// Get the max amount of juice
short Player::GetMaxJuice()
{
	return maxJuice;
}
