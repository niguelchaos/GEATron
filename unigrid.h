#pragma once
#include <set>
#include "game_object.h"
#include "avancezlib.h"
#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include "SDL.h"
#include <unordered_map>
#include "vector2D.h"

class UniGridCell
{
public:
	Vector2D minPos, maxPos;
	//int state; //check gameobject for enum
	//int wallState;
	std::pair<int, int> state; // id = vector index + 1, 2nd is wallstate
	std::vector<GameObject*> cyclesInCell;

	virtual void Init(Vector2D min, Vector2D max);
	virtual void Update(double dt);
};

class UniGrid
{
public:

	int cellSize = 8;
	int windowX, windowY;
	std::vector<GameObject*>* lightcycles;
	//ObjectPool<GameObject>* coll_objects;
	std::vector<GameObject>* grid_coll_objects;
	// vector2d to remember positions of each gridsquare, and objectpool to note whats inside
	std::vector<std::vector<UniGridCell>> grid;


	// Only for pairs of std::hash-able types for simplicity.
	// You can of course template this struct to allow other hash functions
	struct pair_hash {
		template <class T1, class T2>
		std::size_t operator () (const std::pair<T1, T2>& p) const {
			auto h1 = std::hash<T1>{}(p.first);
			auto h2 = std::hash<T2>{}(p.second);

			// Mainly for demonstration purposes, i.e. works but is overly simple
			// In the real world, use sth. like boost.hash_combine
			return h1 ^ h2;
		}
	};

	std::pair<int, int> cellCoord;
	std::unordered_map<std::pair<int, int>, std::vector<GameObject*>, pair_hash> hashTable;
	std::unordered_map<std::pair<int, int>, GameObject*, pair_hash> objectIndex;

	virtual ~UniGrid() { SDL_Log("UniGrid::~UniGrid"); }

	virtual void Init(int cellSize, std::vector<GameObject*>* lightcycles,  int windowX, int windowY);
	virtual void Update(double dt);
	void UpdateState(double dt);
	void UniGrid::CheckCollisions();
	void reset();


};