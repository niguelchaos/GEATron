#pragma once
#include "component.h"
#include "game_object.h"
#include "avancezlib.h"
#include <iostream>

class LightwallBehaviourComponent : public Component
{
	//double cycleXpos;
	//double cycleYpos;
public:
	virtual void Update(float dt);
};

class LightwallRenderComponent : public RenderComponent
{
	std::vector<Sprite*> sprites;
	int direction;
	//PlayerBehaviourComponent* playerBehaviourRef;

public:
	virtual void Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<const char*> sprite_names);
	virtual void Update(float dt);
	virtual void Destroy();
	std::vector<Sprite*> getSprites();
	void setDirection(int direction);
};


class Lightwall : public GameObject
{
	int direction;
public:
	//virtual void Create();
	virtual void Init(double xPos, double yPos, int direction);
	virtual void Receive(Message m);
};




