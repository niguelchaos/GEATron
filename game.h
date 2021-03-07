#pragma once

bool change_direction = false;

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* engine;

	//ObjectPool<Rocket> rockets_pool;	// used to instantiate rockets
	//ObjectPool<Bomb> bombs_pool;
	//ObjectPool<Lightwall> player_lightwall_pool;
	std::vector<Player*> player_pool;
	std::vector<Player*> player_cells_check;

	Player * player;
	
	//ObjectPool<Alien> enemies;
	//Grid * enemies_grid;

	GameObject* window;
	UniGrid* uniformGrid;
	WallController* wallController;

	Sprite * life_sprite;
	bool game_over = false;
	bool game_won = false;
	bool aliens_alive = false;

	unsigned int score = 0;

	int prevTime;
	int currentTime;
	int elapsedTime = 0;

	//float timer = 0;
	//const float TIMER_CD = 0.2f;
	int playerHitboxWidth = 8;
	int playerHitboxHeight = 8;

	int startFrameTime;
	int frameTicksDuration;

	int frameTicksLimit = 1000 / 60;

public:

	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");

		this->engine = engine;
		uniformGrid = new UniGrid();
		prevTime = engine->getElapsedTime();


		player = new Player();
		PlayerBehaviourComponent* player_behaviour = new PlayerBehaviourComponent();
		player_behaviour->Create(engine, player, &game_objects);

		std::vector<const char*> playerSpriteNames;
		playerSpriteNames.push_back("data/UpPlayer.bmp");
		playerSpriteNames.push_back("data/RightPlayer.bmp");
		playerSpriteNames.push_back("data/DownPlayer.bmp");
		playerSpriteNames.push_back("data/LeftPlayer.bmp");
		playerSpriteNames.push_back("data/Explosion1.bmp");
		playerSpriteNames.push_back("data/Explosion2.bmp");
		playerSpriteNames.push_back("data/Explosion3.bmp");
		playerSpriteNames.push_back("data/Explosion4.bmp");

		PlayerRenderComponent* player_render = new PlayerRenderComponent();
		//std::cout << "playerspritenames address" << &playerSpriteNames << std::endl;
		player_render->Create(engine, player, &game_objects, playerSpriteNames, player_behaviour);

		//WallControllerComponent* player_wallcontroller = new WallControllerComponent();
		//player_wallcontroller->Create(engine, player, &game_objects, uniformGrid);

		wallController = new WallController();
		wallController->Create(engine, player, &game_objects, uniformGrid); 

		// Bike Collision Component
		BoxCollideComponent* player_collide = new BoxCollideComponent();
		player_collide->Create(engine, player, &game_objects, (std::vector<GameObject*>*)&player_cells_check, playerHitboxWidth, playerHitboxHeight, uniformGrid);

		player->Create();
		player->AddComponent(player_behaviour);
		//player->AddComponent(player_wallcontroller);
		player->AddComponent(player_render);
		player->AddComponent(player_collide);
		player->AddReceiver(this);

		game_objects.insert(player);

		std::cout << player << std::endl;

		player_pool.push_back(player);

		window = new GameObject();
		window->Create();
		WindowCollideComponent* windowCollider = new WindowCollideComponent();
		windowCollider->Create(engine, window, &game_objects, reinterpret_cast<std::vector<GameObject*>*>(&player_pool), engine->getWidth(), engine->getHeight());
		window->AddComponent(windowCollider);
		game_objects.insert(window);

		//life_sprite = engine->createSprite("data/player.bmp");

		//int totalEnemies = 55;

		//put all aliens into an array
		//for (int i = 0; i < totalEnemies; i++) {
		//enemies.Create(totalEnemies);
		//for (auto alien = enemies.pool.begin(); alien != enemies.pool.end(); alien++)
		//{
		//	//Alien* alien = new Alien();
		//	AlienBehaviourComponent* alien_behaviour = new AlienBehaviourComponent();
		//	alien_behaviour->Create(engine, *alien, &game_objects, &bombs_pool);
		//	RenderComponent* alien_render = new RenderComponent();
		//	alien_render->Create(engine, *alien, &game_objects, "data/enemy_0.bmp");

		//	// Collision
		//	CollideComponent* alien_collide = new CollideComponent();
		//	alien_collide->Create(engine, *alien, &game_objects, (ObjectPool<GameObject>*) & rockets_pool);

		//	(*alien)->Create();
		//	(*alien)->AddComponent(alien_behaviour);
		//	(*alien)->AddComponent(alien_render);
		//	(*alien)->AddComponent(alien_collide);
		//	(*alien)->AddReceiver(this);
		//	(*alien)->enabled = true;
		//}

		// grid of aliens - used to move them all down only once
		// does not need rendercomp because its not drawn to screen
		//enemies_grid = new Grid();
		//GridBehaviourComponent* grid_behaviour = new GridBehaviourComponent();
		//grid_behaviour->Create(engine, enemies_grid, &game_objects, &enemies, &bombs_pool);
		//enemies_grid->Create();
		//enemies_grid->AddComponent(grid_behaviour);
		//enemies_grid->AddReceiver(this);
		//game_objects.insert(enemies_grid);

		//game_objects.insert(player);

	}


	virtual void Init()
	{
		player->Init();
		uniformGrid->Init(8, reinterpret_cast<std::vector<GameObject*>*>(&player_pool), 512, 544);
		window->Init();
		//enemies_grid->Init();
		enabled = true;
	}

	virtual void Update(float dt)
	{
		engine->drawImage("data/phoseh.png");
		AvancezLib::KeyStatus keys;

		startFrameTime = engine->getElapsedTime();

		currentTime = engine->getElapsedTime();
		elapsedTime = currentTime - prevTime;

		//timer += dt;

		if (game_over == true) {
			//dt = 0;
		}

		//std::cout << timer << std::endl;


		engine->getKeyStatus(keys);
		if (keys.esc) {
			Destroy();
			engine->quit();
		}

		uniformGrid->Update(dt);
		wallController->Update(dt);

		//std::cout << "go size: " << game_objects.size() << std::endl;
		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Update(dt);
		

		 //draw each of the cells, aka grid
		//for (int i = 0; i < uniformGrid->grid.size(); i++) {
		//	for (int j = 0; j < uniformGrid->grid[i].size(); j++) {
		//		if (uniformGrid->grid[i][j].state.second == EMPTY) {
		//			//std::cout << "state: " << uniformGrid->grid[i][j].state << std::endl;
		//			engine->drawCell(uniformGrid->grid[i][j].minPos, uniformGrid->grid[i][j].maxPos);
		//		}
		//	}
		//}
	

		// check if aliens are alive - goes through all
		//aliens_alive = false;
		//for (auto alien = enemies.pool.begin(); alien != enemies.pool.end(); alien++)
		//{
		//	if ((*alien)->enabled == true) {
		//		aliens_alive = true;

		//		// also check if any enemy is at bottom of screen
		//		if ((*alien)->verticalPosition >= 480 - 32) {
		//			game_over = true;
		//			player->lives = -1;
		//		}
		//	}
		//}
		// level won if none alive
		if (aliens_alive == false) {
			//game_won = true;
			//game_speed += 0.2;

			//enemies_grid->Init();
			enabled = true;
		}

		frameTicksDuration = engine->getElapsedTime() - startFrameTime;


		//sleep at the end
		if (frameTicksDuration < frameTicksLimit) {
			SDL_Delay(frameTicksLimit - frameTicksDuration); // idk why its always 61
		}
		prevTime = currentTime;

	}

	virtual void Draw()
	{
		//... Draw user interface elements here
		if (game_over == true) {
			showGameover();
		}
		
		showInterface();
		engine->swapBuffers();
		engine->clearWindow();
	}

	virtual void Receive(Message m)
	{
		if (m == GAME_OVER)
		{
			//i dont know why the hell text doesnt appear here
			game_over = true;
			SDL_Log("Game:: OVER");
			SDL_Log("HOTEL:: TRIVAGO");
			for (int i = 0; i < player_pool.size(); i++) {
				if (player_pool[i]->GetComponent<PlayerRenderComponent*>()->isDoneExploding() == true) {
					std::cout << "Done exploding" << std::endl;
					player_pool[i]->enabled = false;
				}
			}
		}

		if (m == ALIEN_HIT) 
		{
			SDL_Log("ALIEN HIT");
			score += POINTS_PER_ALIEN * game_speed;
		}
	}

	virtual void Destroy()
	{
		SDL_Log("Game::Destroy");

		for (auto go = game_objects.begin(); go != game_objects.end(); go++)
			(*go)->Destroy();

		//life_sprite->destroy();
	
		//rockets_pool.Destroy();
		//bombs_pool.Destroy();
		//enemies.Destroy();
		//enemies_grid->Destroy();
		delete player;
	}
	void showGameover() {
		int gameoverXpos = 260; 	int gameoverYpos = 100;
		std::string gameoverString = "=== GAME OVER ===";
		const char* gameoverChar = gameoverString.c_str();
		engine->drawText(gameoverXpos, gameoverYpos, gameoverChar);
	}
	void showGameWon() {
		int levelwonXpos = 260; 	int levelwonYpos = 200;
		std::string levelwonString = "=== LEVEL WON! ===";
		const char* levelwonChar = levelwonString.c_str();
		engine->drawText(levelwonXpos, levelwonYpos, levelwonChar);
	}
	void showInterface() {

		Vector2D headerMin = Vector2D(0, 0);
		Vector2D headerMax = Vector2D(512, 64);
		engine->fillRect(headerMin, headerMax, 150, 100, 200);

		int livesXpos = 40; 	int livesYpos = 25;
		int scoreXpos = 50; 	int scoreYpos = 25;
		int timeXpos = 200; 	int timeYpos = 25;
		//int headerXminpos = 0;	int headerYminpos = 0;
		//int headerXmaxpos = 64;	int headerYmaxpos = 64;

		//std::string livesString = "LIVES LEFT = ";
		//livesString.append(std::to_string(player->lives));
		//const char* livesChar = livesString.c_str();
		//engine->drawText(livesXpos, livesYpos, livesChar);

		std::string scoreString = "SCORE: ";
		scoreString.append(std::to_string(score));
		const char* scoreChar = scoreString.c_str();
		engine->drawText(scoreXpos, scoreYpos, scoreChar);

		std::string timeString = "HISCORE: ";
		timeString.append(std::to_string(game_speed));
		const char* timeChar = timeString.c_str();
		engine->drawText(timeXpos, timeYpos, timeChar);


	}
	void drawBackground() {
		
	}

};