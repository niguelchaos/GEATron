#pragma once

bool change_direction = false;

class Game : public GameObject
{
	std::set<GameObject*> game_objects;	// http://www.cplusplus.com/reference/set/set/
	
	AvancezLib* engine;
	std::vector<Player*> player_pool;
	std::vector<Player*> player_cells_check;

	int currentBackground;
	std::vector<const char*> backgroundNames;

	Player * player;
	Player* player2;
	int winner = 0;

	GameObject* window;
	UniGrid* uniformGrid;
	WallController* wallController;

	bool game_over = false;
	bool game_won = false;
	bool aliens_alive = false;

	unsigned int score = 0;

	int prevTime;
	int currentTime;
	int elapsedTime = 0;

	int playerHitboxWidth = 8;
	int playerHitboxHeight = 8;

	int startFrameTime;
	int frameTicksDuration;

	int frameTicksLimit = 1000 / 60;

	bool prevR = false;
	bool prevB = false;

	bool crashplayed = false;
	bool tictacplayed = false;
	bool erynoiceplayed = false;

public:

	virtual void Create(AvancezLib* engine)
	{
		SDL_Log("Game::Create");
		
		this->engine = engine;
		uniformGrid = new UniGrid();
		prevTime = engine->getElapsedTime();

		// background - supposed to be own component i guess
		currentBackground = 0;

		backgroundNames.push_back("data/background.png");
		backgroundNames.push_back("data/derpakko.jpg");
		backgroundNames.push_back("data/ricardo.jpg");
		backgroundNames.push_back("data/phoseh.png");
		backgroundNames.push_back("data/wideputin.png");
		backgroundNames.push_back("data/menacingcycles.png");
		
		///////////////////////// wallcontroller /////////////////////////
		wallController = new WallController();
		wallController->Create(engine, &game_objects, uniformGrid);

		///////////////////////// window collider /////////////////////////
		window = new GameObject();
		window->Create();
		WindowCollideComponent* windowCollider = new WindowCollideComponent();
		windowCollider->Create(engine, window, &game_objects, reinterpret_cast<std::vector<GameObject*>*>(&player_pool), engine->getWidth(), engine->getHeight());
		window->AddComponent(windowCollider);
		game_objects.insert(window);

		/////////////////////////////////// player 1 /////////////////////////////////////
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
		player_render->Create(engine, player, &game_objects, playerSpriteNames, player_behaviour);

		// Bike Collision Component
		BoxCollideComponent* player_collide = new BoxCollideComponent();
		player_collide->Create(engine, player, &game_objects, (std::vector<GameObject*>*)&player_cells_check, playerHitboxWidth, playerHitboxHeight, uniformGrid, windowCollider);

		player->Create();
		player->AddComponent(player_behaviour);
		player->AddComponent(player_render);
		player->AddComponent(player_collide);
		player->AddReceiver(this);

		game_objects.insert(player);

		std::cout << player << std::endl;

		player_pool.push_back(player);

		//////////////////////////////////// player 2 ////////////////////////////////////
		player2 = new Player();
		Player2BehaviourComponent* player2_behaviour = new Player2BehaviourComponent();
		player2_behaviour->Create(engine, player2, &game_objects);

		// be in player instead separate comp
		std::vector<const char*> player2SpriteNames;
		player2SpriteNames.push_back("data/UpEnemy.bmp");
		player2SpriteNames.push_back("data/RightEnemy.bmp");
		player2SpriteNames.push_back("data/DownEnemy.bmp");
		player2SpriteNames.push_back("data/LeftEnemy.bmp");

		player2SpriteNames.push_back("data/Explosion1.bmp");
		player2SpriteNames.push_back("data/Explosion2.bmp");
		player2SpriteNames.push_back("data/Explosion3.bmp");
		player2SpriteNames.push_back("data/Explosion4.bmp");

		PlayerRenderComponent* player2_render = new PlayerRenderComponent();
		//std::cout << "playerspritenames address" << &playerSpriteNames << std::endl;
		player2_render->Create(engine, player2, &game_objects, player2SpriteNames, player2_behaviour);

		// Bike Collision Component
		BoxCollideComponent* player2_collide = new BoxCollideComponent();
		player2_collide->Create(engine, player2, &game_objects, (std::vector<GameObject*>*) & player_cells_check, playerHitboxWidth, playerHitboxHeight, uniformGrid, windowCollider);

		player2->Create();
		player2->AddComponent(player2_behaviour);
		player2->AddComponent(player2_render);
		player2->AddComponent(player2_collide);
		player2->AddReceiver(this);

		game_objects.insert(player2);

		//std::cout << player2 << std::endl;

		player_pool.push_back(player2);
		////////////////////////////////////////////////////////////////////////

		//////////// sound ///////////////
		engine->PlaySound(0, 1);

	}


	virtual void Init()
	{
		player->Init();
		player2->Init();
		uniformGrid->Init(8, reinterpret_cast<std::vector<GameObject*>*>(&player_pool), 512, 544);
		window->Init();
		enabled = true;
	}

	virtual void Update(float dt)
	{
		AvancezLib::KeyStatus keys;

		startFrameTime = engine->getElapsedTime();

		currentTime = engine->getElapsedTime();
		elapsedTime = currentTime - prevTime;


		if (game_over == true) {
			//dt = 0;
			if (player_pool[1]->GetComponent<PlayerBehaviourComponent*>()->isRekt() == true) {
				winner = 1;
				player_pool[0]->GetComponent<PlayerBehaviourComponent*>()->Pause();
			}
			else if (player_pool[0]->GetComponent<PlayerBehaviourComponent*>()->isRekt() == true) {
				winner = 2;
				player_pool[1]->GetComponent<PlayerBehaviourComponent*>()->Pause();
			}
			if (player_pool[0]->GetComponent<PlayerBehaviourComponent*>()->isRekt() == true && 
					player_pool[1]->GetComponent<PlayerBehaviourComponent*>()->isRekt() == true) {
				winner = 3;
			}
		}

		//std::cout << timer << std::endl;


		engine->getKeyStatus(keys);
		if (keys.esc) {
			Destroy();
			engine->quit();
		}
		if (keys.r == true && prevR == false) {
			//Destroy();
			//Create(this->engine);
			//Init();
			std::cout << "restart" << std::endl;
		}
		prevR = keys.r;

		if (keys.b == true && prevB == false) {
			currentBackground++;
			if (currentBackground >= backgroundNames.size()) {
				currentBackground = 0;
			}
			//std::cout << "next bg" << std::endl;
		}
		prevB = keys.b;
		engine->drawImage(backgroundNames[currentBackground], 0, 64, 512, 480);


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

		
		showInterface();
		if (game_over == true) {
			showGameWon();
		}
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
		delete uniformGrid;
		player->Destroy();
		player2->Destroy();
		window->Destroy();
	}
	void showGameover() {
		int gameoverXpos = 220; 	int gameoverYpos = 32;
		std::string gameoverString = "=== GAME OVER ===";
		const char* gameoverChar = gameoverString.c_str();
		engine->drawText(gameoverXpos, gameoverYpos, gameoverChar);
	}
	void showGameWon() {
		int levelwonXpos = 190; 	int levelwonYpos = 32;
		std::string levelwonString;

		engine->finishSound();
		if (winner == 1) {
			levelwonString = "=== PLAYER 1 WON! ===";
			
			if (crashplayed == false) {
				engine->PlayMp3(0, 1);
				crashplayed = true;
			}
			if (Mix_PlayingMusic() == 0) {
				if (erynoiceplayed == false) {
					engine->PlayMp3(0, 3);
					erynoiceplayed = true;
				}
			}
		}
		if (winner == 2) {
			levelwonString = "=== PLAYER 2 WON! ===";

			if (crashplayed == false) {
				engine->PlayMp3(0, 1);
				crashplayed = true;
			}
			if (Mix_PlayingMusic() == 0) {
				if (tictacplayed == false) {
					engine->PlayMp3(0, 2);
					tictacplayed = true;
					std::cout << "playingmp3" << std::endl;
				}
			}
		}
		if (winner == 3) {
			levelwonString = "=== DRAW! ===";
			if (crashplayed == false) {
				engine->PlayMp3(0, 1);
				crashplayed = true;
			}

		}
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

		std::string scoreString = "SCORE: FOREVER ";
		scoreString.append(std::to_string(score));
		const char* scoreChar = scoreString.c_str();
		engine->drawText(scoreXpos, scoreYpos, scoreChar);

		std::string timeString = "HISCORE: ";
		timeString.append(std::to_string(game_speed));
		const char* timeChar = timeString.c_str();
		//engine->drawText(timeXpos, timeYpos, timeChar);


	}
	void drawBackground() {
		
	}

};