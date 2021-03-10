#pragma once
#include "component.h"
#include "game_object.h"
#include "avancezlib.h"
#include "unigrid.h"
#include <vector>
#include <iostream>

class WallController {
	UniGrid* unigridRef;
	AvancezLib* engine;	// used to access the engine
	//GameObject* go;		// the game object this component is part of
	std::set<GameObject*>* game_objects;	// the global container of game objects


public:
	virtual void Create(AvancezLib* engine, std::set<GameObject*>* game_objects, UniGrid* unigridRef);
	virtual void Update(float dt);
	void drawWalls();
};