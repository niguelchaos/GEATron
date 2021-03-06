#pragma once

#include <set>
#include "object_pool.h"
#include "unigrid.h"

//#include "player.h"

class GameObject;
class AvancezLib;
class Sprite;

class Component
{
protected:
	AvancezLib * engine;	// used to access the engine
	GameObject * go;		// the game object this component is part of
	std::set<GameObject*> * game_objects;	// the global container of game objects

public:
	virtual ~Component() {}

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects);

	virtual void Init() {}
	virtual void Update(float dt) = 0;
	virtual void Receive(int message) {}
	virtual void Destroy() {}
};


class RenderComponent : public Component
{
	Sprite* sprite;

public:

	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, const char * sprite_name);
	virtual void Update(float dt);
	virtual void Destroy();

	Sprite * GetSprite() { return sprite; }
};



class CollideComponent : public Component
{
protected:
	ObjectPool<GameObject> * coll_objects; // collision will be tested with these objects

public:
	virtual void Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects);
	virtual void Update(float dt);
};

class BoxCollideComponent : public CollideComponent
{
	int x;
	int y;
	UniGrid* uniGridref;
	std::vector<UniGridCell> currentCellLocation;
	std::vector<std::pair<int, int>> currentHashCellLocation; // i really think this is the object index instead

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<GameObject>* coll_objects, int x, int y, UniGrid* uniGrid);
	virtual void Update(double dt);
	void InsertCurrentCell(UniGridCell cell);
};

