#include "Player.h"
#include "P_Teleportation.h"

Player::Player(GameObject* gameObject) : UserComponent(gameObject)
{
	fpm = gameObject->GetComponent<FirstPersonMovement>();
	maxJuice = 8;
	currentJuice = maxJuice;

	//Setup powers
	primary = new P_Teleportation();
	secondary = nullptr;
	tertiary = nullptr;
}

Player::~Player() 
{
	if (primary != nullptr)
		delete primary;
	if (secondary != nullptr)
		delete secondary;
	if (tertiary != nullptr)
		delete tertiary;
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
	//Move the player from powers
	if (primary != nullptr)
		primary->MoveCharacter(*this);
	if (secondary != nullptr)
		secondary->MoveCharacter(*this);
	if (tertiary != nullptr)
		secondary->MoveCharacter(*this);
}

void Player::Update(float deltaTime)
{
	//Update all the powers
	if (primary != nullptr)
		primary->Update(*this, PowerSlot::Primary, currentJuice);
	if (secondary != nullptr)
		secondary->Update(*this, PowerSlot::Secondary, currentJuice);
	if (tertiary != nullptr)
		tertiary->Update(*this, PowerSlot::Tertiary, currentJuice);
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