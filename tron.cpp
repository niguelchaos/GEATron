#define _CRT_SECURE_NO_WARNINGS

#include "stdio.h"
#include "avancezlib.h"
#include <cstdlib>
#include <set>

const unsigned int	MAX_NUM_GAME_OBJECTS = 10000;

float game_speed = 1.f;		// speed of the game; it is increased each time all the aliens are hit
							// it is also the score multiplier

#include "vector2D.h"
#include "component.h"
#include "game_object.h"

#include "unigrid.h"
#include "player.h"
#include "wallcontroller.h"

#include "iostream"
#include <random>

#include <string>
#include "game.h"
#include <time.h>



int main(int argc, char** argv)
{

	AvancezLib engine;

	engine.init(512, 544);

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



