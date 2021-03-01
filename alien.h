#pragma once

class Alien : public GameObject
{
public:

	int lives;	// it's game over when goes below zero 
	int* hitEdge; //  0 for going right, 1 = left
	int* stepDown;

	virtual ~Alien() { SDL_Log("Alien::~Alien"); }

	virtual void Init(int* stepdownFlag, int* hitEdgeFlag)
	{
		SDL_Log("Alien::Init");
		GameObject::Init();
		lives = NUM_ALIEN_LIVES;
		hitEdge = 0;
		this->stepDown = stepdownFlag;
		this->hitEdge = hitEdgeFlag;
		enabled = true;
	}

	virtual void Receive(Message m)
	{
		if (m == HIT)
		{
			SDL_Log("Alien::Hit!");
			Send(ALIEN_HIT);
			RemoveLife();

			if (lives < 0) {
				//Send(GAME_OVER);
				enabled = false;
			}

		}
		if (m == STEPDOWN) {
			//SDL_Log("Alien::Stepdown");
			verticalPosition += ALIEN_DOWN_SPEED;
		}
	}

	void RemoveLife()
	{
		lives--;
		SDL_Log("remaining lives %d", lives);
	}

	void StepDown() {
		//SDL_Log("Alien::Stepdown");
		verticalPosition += ALIEN_DOWN_SPEED;
	}
};

class AlienBehaviourComponent : public Component
{
	float time_fire_pressed;	// time from the last time the fire button was pressed
	ObjectPool<Bomb>* bombs_pool;

public:
	virtual ~AlienBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Bomb>* bombs_pool)
	{
		Component::Create(engine, go, game_objects);
		this->bombs_pool = bombs_pool;
	}

	virtual void Init()
	{
		time_fire_pressed = -10000.f;
	}

	virtual void Update(float dt)
	{
		Alien* alien = (Alien*)go;

		if (go->horizontalPosition <= 0 && *(alien->hitEdge) == 1) {
			*(alien->hitEdge) = 0;
			*(alien->stepDown) = 1;
		}
		if (go->horizontalPosition >= (640 - 32) && *(alien->hitEdge) == 0) {
			*(alien->hitEdge) = 1;
			*(alien->stepDown) = 1;
		}

		if (go->verticalPosition >= (480)) {
			go->verticalPosition = 480;
		}
		// hasnt hit right edge
		if (*alien->hitEdge == 0) {
			Move(ALIEN_SPEED * dt);
			//std::cout << dt << std::endl;
		}
		// has hit right edge, coming back
		if (*alien->hitEdge == 1) {
			Move(-ALIEN_SPEED * dt);
		}

		//if (CanFire()) {
		//	// fetches a bomb from the pool and use it in game_objects
		//	Bomb* bomb = bombs_pool->FirstAvailable();
		//	if (bomb != NULL)	// rocket is NULL is the object pool can not provide an object
		//	{
		//		bomb->Init(go->horizontalPosition, go->verticalPosition);
		//		game_objects->insert(bomb);
		//	}
		//}

	}


	// move the player left or right
	// param move depends on the time, so the player moves always at the same speed on any computer
	void Move(float move)
	{
		go->horizontalPosition += move;

		if (go->horizontalPosition > (640 - 32)) {
			go->horizontalPosition = 640 - 32;
		}


		if (go->horizontalPosition < 0) {
			go->horizontalPosition = 0;
		}

	}
	// return true if enough time has passed from the previous rocket
	//bool CanFire()
	//{
	//	// shoot just if enough time passed by
	//	if ((engine->getElapsedTime() - time_fire_pressed) < (BOMB_TIME_INTERVAL / game_speed)) {
	//		return false;
	//	}
	//	else {
	//		std::random_device rd; // obtain a random number from hardware
	//		std::mt19937 gen(rd()); // seed the generator
	//		std::uniform_int_distribution<> distr(0, 1); // define the range

	//		//std::cout << randBomb << ' '; 

	//		time_fire_pressed = engine->getElapsedTime();

	//		int randBomb = distr(gen);
	//		if (randBomb == 1) {
	//			//SDL_Log("BOMB!");
	//			return true;
	//		}
	//	}
	//	return false;
	//}
};