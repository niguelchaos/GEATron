#pragma once


class BombBehaviourComponent : public Component
{
public:

	void Update(float dt)
	{
		go->verticalPosition += (BOMB_SPEED + (go->verticalPosition)) * dt; // Bomb_speed * acceleration * time

		if (go->verticalPosition > 480) // When the Bomb reaches the bot of the screen, it disappears.
			go->enabled = false;
	}
};


// Bombs are shot by the aliens towards the player
class Bomb : public GameObject
{

public:

	virtual void Init(double xPos, double yPos)
	{
		SDL_Log("Bomb::Init");
		GameObject::Init();

		horizontalPosition = xPos;
		verticalPosition = yPos;
	}

	virtual void Receive(Message m)
	{
		if (!enabled)
			return;

		if (m == HIT)
		{
			enabled = false;
			SDL_Log("Bomb::Hit");
		}
	}
};