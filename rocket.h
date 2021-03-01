#pragma once
#include "component.h"
#include "game_object.h"

class RocketBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->verticalPosition -= 100 * dt; // rocket_speed * time

		if (go->verticalPosition < 0) // When the rocket reaches the top of the screen, it disappears.
			go->enabled = false;
	}
};


// rockets are shot by the player towards the aliens
class Rocket : public GameObject
{

public:

	virtual void Init(double xPos)
	{
		SDL_Log("Rocket::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = 480 - 52;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Rocket::Hit");
		}
	}
};