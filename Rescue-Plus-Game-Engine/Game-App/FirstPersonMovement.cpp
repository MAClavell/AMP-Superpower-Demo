#include "FirstPersonMovement.h"
#include "ExtendedMath.h"
#include "PhysicsManager.h"

using namespace DirectX;

#define MOVE_SPEED 10
#define SPRINT_SPEED 14
#define CROUCH_SPEED 5
#define SLIDE_SPEED (SPRINT_SPEED * 200)
#define SLIDE_FRICTION (SLIDE_SPEED * 0.005f)
#define SLIDE_CUTOFF 1.0f
#define JUMP_SPEED 1300
#define STAND_HEIGHT 2.0f
#define CROUCH_HEIGHT 1.5f
#define SLIDE_HEIGHT 1.0f

FirstPersonMovement::FirstPersonMovement(GameObject* gameObject) : UserComponent(gameObject)
{
	inputManager = InputManager::GetInstance();

	controller = gameObject->GetComponent<CharacterController>();

	auto children = gameObject->GetChildren();

	//Get Camera
	camera = children[0]->GetComponent<Camera>();
	cameraGO = children[0];

	sprinting = false;
	grounded = false;
	crouching = false;
	sliding = false;
	firstSlideFrame = false;
	applyGravity = true;

	velocity = XMFLOAT3(0, 0, 0);
	slideDir = XMFLOAT3(0, 0, 0);
}
FirstPersonMovement::~FirstPersonMovement()
{ }

// Factory function to safely create a First Person Movement object
FirstPersonMovement* FirstPersonMovement::CreateFirstPersonCharacter(const char* name,
	int screenWidth, int screenHeight)
{
	//Root object
	GameObject* root = new GameObject(name);
	root->AddComponent<CharacterController>(1, STAND_HEIGHT);

	//Camera object
	GameObject* camera = new GameObject("FPCamera");
	camera->AddComponent<Camera>()
		->CreateProjectionMatrix(0.25f * XM_PI, (float)screenWidth / screenHeight, 0.1f, 10000.0f);
	camera->SetParent(root);
	camera->SetLocalPosition(0, 2, 0);

	//Add FirstPersonMovement component
	FirstPersonMovement* fps = root->AddComponent<FirstPersonMovement>();
	root->SetPosition(0, 2.5f, 0);
	return fps;
}

void FirstPersonMovement::Update(float deltaTime)
{
	//Rotate the camera to where the user is looking
	inputManager->CaptureWindow();
	CalculateCameraRotFromMouse();

	//Detect Input first
	grounded = controller->IsGrounded();
	short movementZ = 0; //0=none, 1=W, 2=S
	short movementX = 0; //0=none, 1=D, 2=A
	if (inputManager->GetKey(Key::W))
	{
		movementZ = 1;
	}
	else if (inputManager->GetKey(Key::S))
	{
		movementZ = 2;
	}
	if (inputManager->GetKey(Key::D))
	{
		movementX = 3;
	}
	else if (inputManager->GetKey(Key::A))
	{
		movementX = 4;
	}

	//Initialize physics
	XMVECTOR accVec = XMVectorSet(0, 0, 0, 0);
	XMVECTOR velVec = XMLoadFloat3(&velocity);

	//Sprint check
	//Don't have to hold the sprint buttons down
	//Must be moving forward
	//Toggle
	if (grounded && inputManager->GetKeyDown(Key::LeftShift) && !sprinting && movementZ == 1)
	{
		if (crouching)
			EndCrouch();
		else if (sliding)
			EndSlide(&velVec, &accVec);
		
		StartSprint();
	}
	else if (sprinting && movementZ == 0)
		EndSprint();

	//Crouch check
	//Toggle
	if (grounded && inputManager->GetKeyDown(Key::LeftControl))
	{
		//If we are sprinting, start a slide
		if (crouching)
		{
			EndCrouch();
		}
		else if (sprinting)
		{
			EndSprint();
			StartSlide(&accVec);
		}
		else if (sliding)
		{
			EndSlide(&velVec, &accVec);
			StartCrouch();
		}
		else StartCrouch();
	}

	//Jumping
	if (grounded && inputManager->GetKeyDown(Key::Space))
	{
		if (crouching)
			EndCrouch();
		else if (sliding)
			EndSlide(&velVec, &accVec);

		velVec = XMVectorSetY(velVec, 0);
		accVec = XMVectorSetY(accVec, JUMP_SPEED);
	}

	//Get correct speed mult
	float currSpeed = MOVE_SPEED;
	if (sprinting)
		currSpeed = SPRINT_SPEED;
	else if (crouching)
		currSpeed = CROUCH_SPEED;

	//Apply movement
	XMVECTOR moveVec = XMVectorSet(0, 0, 0, 0);
	if (!sliding)
	{
		//Relative Z movement
		//W
		if (movementZ == 1)
		{
			XMFLOAT3 forward = gameObject()->GetForwardAxis();
			moveVec = XMVectorAdd(moveVec,
				XMVectorScale(XMLoadFloat3(&forward), currSpeed * deltaTime));
		}
		//S
		else if (movementZ == 2)
		{
			XMFLOAT3 forward = gameObject()->GetForwardAxis();
			moveVec = XMVectorSubtract(moveVec,
				XMVectorScale(XMLoadFloat3(&forward), currSpeed * deltaTime));
		}
		//Relative X movement
		//D
		if (movementX == 3)
		{
			XMFLOAT3 right = gameObject()->GetRightAxis();
			moveVec = XMVectorAdd(moveVec,
				XMVectorScale(XMLoadFloat3(&right), currSpeed * deltaTime));
		}
		//A
		else if (movementX == 4)
		{
			XMFLOAT3 right = gameObject()->GetRightAxis();
			moveVec = XMVectorSubtract(moveVec,
				XMVectorScale(XMLoadFloat3(&right), currSpeed * deltaTime));
		}
	}
	//Sliding movement
	else
	{
		if (!firstSlideFrame)
		{
			//If we stopped sliding from velocity
			float velX = XMVectorGetX(velVec);
			float velZ = XMVectorGetZ(velVec);
			if (velX < SLIDE_CUTOFF && velX > -SLIDE_CUTOFF
				&& velZ < SLIDE_CUTOFF && velZ > -SLIDE_CUTOFF)
			{
				EndSlide(&velVec, &accVec);
				StartCrouch();
			}
			//Apply friction
			else accVec = XMVectorSubtract(accVec, XMVectorScale(XMLoadFloat3(&slideDir), SLIDE_FRICTION));
		}
		else firstSlideFrame = false;
	}
	
	//Apply custom physics
	velVec = XMVectorAdd(velVec, XMVectorScale(accVec, deltaTime));

	//Apply gravity
	if (applyGravity && !grounded)
	{
		velVec = XMVectorSetY(velVec, XMVectorGetY(velVec) + (PhysicsManager::GetInstance()->GetGravity() * deltaTime));
	}
	XMStoreFloat3(&velocity, velVec);
	moveVec = XMVector3ClampLength(moveVec, 0, currSpeed * deltaTime);
	moveVec = XMVectorAdd(moveVec, XMVectorScale(velVec, deltaTime));

	//Move the character
	XMFLOAT3 move;
	XMStoreFloat3(&move, moveVec);
	controller->Move(move, deltaTime, false);
}

// Changes for when we start a sprint
void FirstPersonMovement::StartSprint()
{
	sprinting = true;
}
// Changes for when we end a sprint
void FirstPersonMovement::EndSprint()
{
	sprinting = false;
}

// Changes for when we start a crouch
void FirstPersonMovement::StartCrouch()
{
	crouching = true;
	controller->Resize(CROUCH_HEIGHT);
	cameraGO->SetLocalPosition(0, CROUCH_HEIGHT, 0);
}
// Changes for when we end a crouch
void FirstPersonMovement::EndCrouch()
{
	crouching = false;
	controller->Resize(STAND_HEIGHT);
	cameraGO->SetLocalPosition(0, STAND_HEIGHT, 0);
}

// Changes for when we start a slide
void FirstPersonMovement::StartSlide(XMVECTOR* accVec)
{
	sliding = true;
	firstSlideFrame = true;
	*accVec = XMVectorAdd(*accVec, 
		XMVectorScale(XMLoadFloat3(&gameObject()->GetForwardAxis()), SLIDE_SPEED));
	slideDir = gameObject()->GetForwardAxis();
	controller->Resize(SLIDE_HEIGHT);
	cameraGO->SetLocalPosition(0, SLIDE_HEIGHT, 0);

}
// Changes for when we end a slide
void FirstPersonMovement::EndSlide(XMVECTOR* velVec, XMVECTOR* accVec)
{
	sliding = false;
	*velVec = XMVectorSet(0, XMVectorGetY(*velVec), 0, 0);
	*accVec = XMVectorSet(0, XMVectorGetY(*accVec), 0, 0);
	controller->Resize(STAND_HEIGHT);
	cameraGO->SetLocalPosition(0, STAND_HEIGHT, 0);
}

// Calculate the camera's rotation when the player moves the mouse
void FirstPersonMovement::CalculateCameraRotFromMouse()
{
	static float cameraSensitivity = 0.15f;

	//Get the center position of the window
	long centerX = inputManager->GetWindowCenterX();
	long centerY = inputManager->GetWindowCenterY();

	//Get the mouse position
	long mouseX = inputManager->GetMouseX();
	long mouseY = inputManager->GetMouseY();

	//Calculate the difference in view with the angle
	float fAngleX = 0.0f;
	float fAngleY = 0.0f;
	float fDeltaMouse = 0.0f;
	if (mouseX > centerX)
	{
		fDeltaMouse = static_cast<float>(mouseX - centerX);
		fAngleY += fDeltaMouse * cameraSensitivity;
	}
	else if (mouseX < centerX)
	{
		fDeltaMouse = static_cast<float>(centerX - mouseX);
		fAngleY -= fDeltaMouse * cameraSensitivity;
	}

	if (mouseY > centerY)
	{
		fDeltaMouse = static_cast<float>(centerY - mouseY);
		fAngleX -= fDeltaMouse * cameraSensitivity;
	}
	else if (mouseY < centerY)
	{
		fDeltaMouse = static_cast<float>(mouseY - centerY);
		fAngleX += fDeltaMouse * cameraSensitivity;
	}

	static float xRot = 0;

	xRot += fAngleX;
	yRot += fAngleY;

	//Keep camera from reversing when looking up/down
	if (xRot > 89.9f)
		xRot = 89.9f;
	if (xRot < -89.9f)
		xRot = -89.9f;

	//Change the Yaw and the Pitch of the camera
	gameObject()->SetRotation(0, yRot, 0);
	cameraGO->SetRotation(xRot, yRot, 0);

	SetCursorPos(centerX, centerY); //Position the mouse in the center
	SetCursor(false);
}

// Get the controller's camera
Camera* FirstPersonMovement::GetCamera()
{
	return camera;
}
