#include "lightwall.h"


void LightwallBehaviourComponent::Update(float dt)
{
	go->verticalPosition = go->verticalPosition; // rocket_speed * time
	//std::cout << "UPDATED POS = " << go->horizontalPosition << ", " << go->verticalPosition << std::endl;
	//if (go->verticalPosition < 0) // When the rocket reaches the top of the screen, it disappears.
	//	go->enabled = false;
}

void Lightwall::Init(double xPos, double yPos, int direction)
{
	SDL_Log("Lightwall::Init");
	GameObject::Init();

	this->horizontalPosition = xPos;
	this->verticalPosition = yPos;
	this->direction = direction;

	LightwallRenderComponent* render = this->GetComponent<LightwallRenderComponent*>();
	render->setDirection(direction);
	if (direction == 0 || direction == 2) {
		//std::cout << "UP/DOWN" << xPos << ", " << yPos << std::endl;
		render->getSprites()[0]->draw(int(this->horizontalPosition), int(this->verticalPosition));
	}
	if (direction == 1 || direction == 3) {
		//std::cout << " <<<  LEFT/ RIGHT >>>" << xPos << ", " << yPos << std::endl;
		render->getSprites()[1]->draw(int(xPos), int(yPos));
	}
	
}

void Lightwall::Receive(Message m)
{
	if (!enabled)
		return;

	if (m == HIT)
	{
		enabled = false;
		SDL_Log("Lightwall::Hit");
	}
}

void LightwallRenderComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<const char*> sprite_names)
{
	Component::Create(engine, go, game_objects);
	
	for (int i = 0; i < sprite_names.size(); i++) {
		Sprite* sprite = engine->createSprite(sprite_names[i]);
		sprites.push_back(sprite);
	}
}

void LightwallRenderComponent::Update(float dt) {
	if (direction == 0 || direction == 2) {
		//std::cout << "UP/DOWN" << xPos << ", " << yPos << std::endl;
		getSprites()[0]->draw(int(go->horizontalPosition), int(go->verticalPosition));
	}
	if (direction == 1 || direction == 3) {
		//std::cout << " <<<  LEFT/ RIGHT >>>" << xPos << ", " << yPos << std::endl;
		getSprites()[1]->draw(int(go->horizontalPosition), int(go->verticalPosition));
	}
}

std::vector<Sprite*> LightwallRenderComponent::getSprites()
{
	return sprites;
}

void LightwallRenderComponent::setDirection(int direction) {
	this->direction = direction;
}

void LightwallRenderComponent::Destroy()
{
	for (int i = 0; i < sprites.size(); i++) {
		if (sprites[i] != NULL)
			sprites[i]->destroy();
		sprites[i] = NULL;
	}
}