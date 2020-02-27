#include "Player.h"

Player::Player(GameObject* gameObject) : UserComponent(gameObject)
{
	fpm = gameObject->GetComponent<FirstPersonMovement>();
	maxJuice = 8;
	currentJuice = maxJuice;

	//Setup powers
	primary = nullptr;
	secondary = nullptr;
	tertiary = nullptr;
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

void Player::FixedUpdate(float fixedTimestep)
{
}

void Player::Update(float deltaTime)
{
	//Update all the powers
	if (primary != nullptr)
		primary->Update(PowerSlot::Primary, currentJuice);
	if (secondary != nullptr)
		secondary->Update(PowerSlot::Secondary, currentJuice);
	if (tertiary != nullptr)
		tertiary->Update(PowerSlot::Tertiary, currentJuice);
}

// Get the camera attached to this player
Camera* Player::GetCamera()
{
	return fpm->GetCamera();
}