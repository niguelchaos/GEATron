class GridBehaviourComponent : public Component
{
	float time_fire_pressed;
	int stepdownFlag = 0;
	int hitEdgeFlag = 0;


	ObjectPool<Alien>* enemies_pool;
	ObjectPool<Bomb>* bombs_pool;

public:
	virtual ~GridBehaviourComponent() {}

	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Alien>* enemies_pool, ObjectPool<Bomb>* bombs_pool)
	{
		Component::Create(engine, go, game_objects);

		this->enemies_pool = enemies_pool;
		this->bombs_pool = bombs_pool;
	}

	virtual void Init()
	{
		time_fire_pressed = -10000.f;	// time from the last time a bomb was dropped by one of the aliens
		// Alien
		int prevAlienHorizPos = 1;
		int prevAlienVertPos = 40;
		int currentAlienHorizPos = 0;
		int currentAlienVertPos = 40;

		for (auto alien = enemies_pool->pool.begin(); alien != enemies_pool->pool.end(); alien++)
		{
			(*alien)->Init(&stepdownFlag, &hitEdgeFlag);

			int index = alien - enemies_pool->pool.begin();

			// i have no idea how to grid ima kill myself thanks
			if (index % 11 == 0) {
				if (index != 0) {
					currentAlienVertPos = prevAlienVertPos + 35;
					prevAlienVertPos = currentAlienVertPos;
					// reset horizontal
					prevAlienHorizPos = 1;
					currentAlienHorizPos = 0;
				}
			}
			currentAlienHorizPos = prevAlienHorizPos + 32;

			(*alien)->horizontalPosition = currentAlienHorizPos;
			(*alien)->verticalPosition = currentAlienVertPos;
			game_objects->insert(*alien);

			prevAlienHorizPos = currentAlienHorizPos;
		}
	}

	virtual void Update(float dt)
	{
		// somehow all aliens are linked to 1 variable?
		// yea im really stuck here
		// cant wait for lab 5 fkkkk i want insurance
		if (stepdownFlag == 1) {
			for (auto alien = enemies_pool->pool.begin(); alien != enemies_pool->pool.end(); alien++) {
				if ((*alien)->enabled) {
					(*alien)->StepDown();
				}
			}
			SDL_Log("Grid Stepdown");
		}	

			
		// is enough time passed from the last bomb, shoot another bomb from a random active alien
		if (CanFire())
		{
			if (dt != 0) {
				Bomb* bomb = bombs_pool->FirstAvailable();
				if (bomb != NULL)
				{
					Alien* alien = enemies_pool->SelectRandom();
					if (alien != NULL)
					{
						bomb->Init(alien->horizontalPosition, alien->verticalPosition);
						game_objects->insert(bomb);
					}
				}
			}
		}
		// make sure its false after so stepdown only happens once
		stepdownFlag = false;
	}

	// return true if enough time has passed from the previous bomb
	bool CanFire()
	{
		// shoot just if enough time passed by
		if ((engine->getElapsedTime() - time_fire_pressed) < (BOMB_TIME_INTERVAL / game_speed)) {
			return false;
		}

		// drop the bomb with 90% chance
		if ((rand() / (float)RAND_MAX) < 0.1f) {
			//SDL_Log("bomb DENIED");
			return false;
		}


		time_fire_pressed = engine->getElapsedTime();
		return true;
	}
};



class Grid : public GameObject
{
public:

	virtual ~Grid() { SDL_Log("Grid::~Grid"); }

	virtual void Init()
	{
		SDL_Log("Grid::Init");
		GameObject::Init();
	}

};