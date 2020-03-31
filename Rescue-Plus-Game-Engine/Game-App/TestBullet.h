#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"
#include "InputManager.h"

class TestBullet :
	public UserComponent
{
private:
	Mesh* sphereMesh;
	Material* whiteMat;
	InputManager* inputManager;

public:
	TestBullet(GameObject* gameObject);
	~TestBullet();

	void Update() override;
};

