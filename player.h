#pragma once
#include "component.h"
#include "game_object.h"
#include "avancezlib.h"
#include "rocket.h"
#include "lightwall.h"
#include "unigrid.h"
#include <vector>
#include <iostream>



class PlayerBehaviourComponent : public Component
{
	float timer = 0;
	const float TIMER_CD = 0.05f;

	float time_fire_pressed;	// time from the last time the fire button was pressed
	float fire_time_interval = .1f;

	int gear = 1; // 0 = lowest speed, 2 = highest
	float speed = 16;

	bool prevKeyRight = false;
	bool prevKeyLeft = false;
	int currentDirection = 0; // 0 = up, 1 = right, clockwise
	int prevDirection = 0;

	int prevPosX;
	int prevPosY;

	bool prevKeyUp = false;
	bool prevKeyDown = false;


	float time_turn_pressed;	// time from the last time the right/left button was pressed
	float TURN_COOLDOWN = 1.f;
	const int RIGHT = 1;
	const int LEFT = -1;
	const int ACCELERATE = 1;
	const int DECELERATE = -1;
	ObjectPool<Lightwall> * lightwall_pool;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects);
	virtual void Init();
	virtual void Update(float dt);

	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void ChangeDirection(int direction);
	void ChangeSpeed(int change);

	float getCurrentDirection();
	float getPrevDirection();
	int getGear();


	// return true if enough time has passed from the previous rocket
	bool CanFire();

};

class PlayerRenderComponent : public RenderComponent
{
	std::vector<Sprite*> sprites;
	PlayerBehaviourComponent* playerBehaviourRef;

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<const char*> sprite_names, PlayerBehaviourComponent* playerBehaviourRef);
	virtual void Update(float dt);
	virtual void Destroy();
};

// the main player
class Player : public GameObject
{
public:

	int lives;	// it's game over when goes below zero 

	virtual ~Player()	{		SDL_Log("Player::~Player");	}

	virtual void Init();
	virtual void Receive(Message m);

	void RemoveLife();

};



