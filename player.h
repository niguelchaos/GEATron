#pragma once
#include "component.h"
#include "game_object.h"
#include "avancezlib.h"
#include "unigrid.h"
#include <vector>
#include <iostream>



class PlayerBehaviourComponent : public Component
{
protected:
	float timer = 0;
	const float TIMER_CD = 0.05f; 
	bool paused = false;


	int gear = 1; // 0 = lowest speed, 2 = highest
	float speed = 16;

	bool prevKeyRight = false;
	bool prevKeyLeft = false;
	bool prevKeyUp = false;
	bool prevKeyDown = false;

	bool prevKeyd = false;
	bool prevKeya = false;
	bool prevKeyw = false;
	bool prevKeys = false;

	int currentDirection = 0; // 0 = up, 1 = right, clockwise
	int prevDirection = 0;

	int prevPosX;
	int prevPosY;

	int playerNum = 0;
	int startX; int startY;

	const int RIGHT = 1;
	const int LEFT = -1;
	const int ACCELERATE = 1;
	const int DECELERATE = -1;

	
	bool rekt = false;

public:
	virtual ~PlayerBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects);
	virtual void Init();
	virtual void Update(float dt);
	virtual void Receive(Message m);

	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void ChangeDirection(int direction);
	void ChangeSpeed(int change);

	float getCurrentDirection();
	float getPrevDirection();
	int getGear();
	void Pause();
	bool isRekt();
	void playRektMp3();

};

class Player2BehaviourComponent : public PlayerBehaviourComponent
{
public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects);
	//virtual void Init();
	virtual void Update(float dt);
	//virtual void Receive(Message m);
};

class PlayerRenderComponent : public RenderComponent
{
	std::vector<Sprite*> sprites;
	PlayerBehaviourComponent* playerBehaviourRef;
	bool isRekt = false;
	int explosionAnimationFrame = 0;
	bool doneExploding = false;

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<const char*> sprite_names, PlayerBehaviourComponent* playerBehaviourRef);
	virtual void Update(float dt);
	virtual void Destroy();
	virtual void Receive(Message m);
	bool isDoneExploding();
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



