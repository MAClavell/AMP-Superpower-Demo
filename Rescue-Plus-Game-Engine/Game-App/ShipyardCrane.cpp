#include "ShipyardCrane.h"
#include "GameObject.h"
#include <math.h>
#include "Times.h"
#define RANGE 12.0f
#define SPEED 2

ShipyardCrane::ShipyardCrane(GameObject* gameObject, GameObject* hook) : UserComponent(gameObject)
{ 
	this->hook = hook;
}

ShipyardCrane::~ShipyardCrane()
{ }

void ShipyardCrane::Update()
{
	static float x = 0;
	static short mult = 1;

	//Increment and reverse
	x += SPEED * mult * Time::deltaTime();
	if (abs(x) > RANGE)
	{
		x = mult * RANGE;
		mult *= -1;
	}

	hook->SetLocalPosition(x, 16.5f, 0);
}