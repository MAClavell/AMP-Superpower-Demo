#pragma once
#include "Camera.h"
#include "InputManager.h"

// --------------------------------------------------------
// A first person camera definition. Includes flying movement
// --------------------------------------------------------
class DebugMovement : public UserComponent
{
private:
	InputManager* inputManager;

	//Allow first person movement in debug mode
	float moveSpeed = 10;
	bool prevKeypress;

	// --------------------------------------------------------
	// Calculate the camera's rotation when moving
	// --------------------------------------------------------
	void CalculateCameraRotFromMouse();

	// --------------------------------------------------------
	// Allow fps movement for debugging
	// --------------------------------------------------------
	void Movement();

public:
	// --------------------------------------------------------
	// Constructor - Set up the debug camera
	// (Remember to create the projection matrix right after!)
	// --------------------------------------------------------
	DebugMovement(GameObject* gameObject);

	// --------------------------------------------------------
	// Destructor for when an instance is deleted
	// --------------------------------------------------------
	~DebugMovement();

	// --------------------------------------------------------
	// Update the movement (runs every frame)
	// --------------------------------------------------------
	void Update() override;
};

