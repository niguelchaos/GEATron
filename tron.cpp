#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include "object_pool.h"
#include <cstdlib>
#include <set>



const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;
const unsigned int	NUM_LIVES = 2;
const unsigned int  NUM_ALIEN_LIVES = 0;

const unsigned int	MAX_NUM_ROCKETS = 32;
const unsigned int	MAX_NUM_BOMBS = 32;
const unsigned int	POINTS_PER_ALIEN = 100;
const float			FIRE_TIME_INTERVAL = .1f;
const float			BOMB_TIME_INTERVAL = .25f;
const float			PLAYER_SPEED = 160.0f;
const float			ROCKET_SPEED = 240.0f;
const float			ALIEN_SPEED = 100.0f;
const float			ALIEN_DOWN_SPEED = 30.0f;
const float			BOMB_SPEED = 120.0f;


float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier

#include "vector2D.h"
#include "component.h"
#include "game_object.h"


#include "rocket.h"
#include "lightwall.h"

#include "unigrid.h"
#include "player.h"
#include "wallcontroller.h"



#include "bomb.h"
#include "iostream"
#include <random>
#include "alien.h"
#include "grid.h"

#include <string>
#include "game.h"
#include <time.h>





int main(int argc, char** argv)
{

	AvancezLib engine;

	engine.init(512, 544);
	//engine.init(1024, 960);

	Game game;
	game.Create(&engine);
	game.Init();

	float lastTime = engine.getElapsedTime();
	while (true)
	{
		float newTime = engine.getElapsedTime();
		float dt = newTime - lastTime;
		dt = dt * game_speed;
		lastTime = newTime;

		engine.processInput();
		game.Update(dt);
		game.Draw();
	}

	// clean up
	game.Destroy();
	engine.destroy();

	return 0;
}



