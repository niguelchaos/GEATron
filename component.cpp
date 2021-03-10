#include "component.h"
#include "game_object.h"
#include "avancezlib.h"
#include "player.h"

void Component::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects)
{
	this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;
}

void RenderComponent::Create(AvancezLib * engine, GameObject * go, std::set<GameObject*>* game_objects, const char * sprite_name)
{
	Component::Create(engine, go, game_objects);

	sprite = engine->createSprite(sprite_name);
}

void RenderComponent::Update(float dt)
{
	if (!go->enabled)
		return;

	if (sprite)
		sprite->draw(int(go->horizontalPosition), int(go->verticalPosition));
}

void RenderComponent::Destroy()
{
	if (sprite != NULL)
		sprite->destroy();
	sprite = NULL;
}



void CollideComponent::Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, ObjectPool<GameObject> * coll_objects)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
}


void CollideComponent::Update(float dt)
{

}

void BoxCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<GameObject*>* coll_objects, int x, int y, UniGrid* uniGrid, WindowCollideComponent* windowCollider) {
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
	this->minX = go->horizontalPosition;
	this->minY = go->verticalPosition;
	this->maxX = minX + x;
	this->maxY = minY + y;
	this->uniGridref = uniGrid;
	this->windowCollider = windowCollider;
}

int BoxCollideComponent::getMinX() { return minX; }
int BoxCollideComponent::getMinY() { return minY; }
int BoxCollideComponent::getMaxX() { return maxX; }
int BoxCollideComponent::getMaxY() { return maxY; }

void BoxCollideComponent::Update(float dt)
{

	// take game objects x
	BoxCollideComponent* boxCollider = go->GetComponent<BoxCollideComponent*>();
	PlayerBehaviourComponent* playerBehaviourRef = go->GetComponent<PlayerBehaviourComponent*>();

	int hitbox = 8;
	int offset = 9;

	minX = go->horizontalPosition;
	minY = go->verticalPosition;
	maxX = go->horizontalPosition;
	maxY = (go->verticalPosition ); 



	if (playerBehaviourRef->getGear() == 1) {
		offset = 16;
	}
	if (playerBehaviourRef->getGear() == 2) {
		hitbox = 14;
		offset = 24;
	}

	if (playerBehaviourRef->getCurrentDirection() == 0) {
		maxY -= offset;
		minY = maxY - hitbox;
		maxX += hitbox - 6;
		engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
		//std::cout << "			|| MIN:  " << minX << " , " << minY << " || MAX: " << maxX << ", " << maxY << std::endl;
	}
	if (playerBehaviourRef->getCurrentDirection() == 1) {
		maxX += offset + hitbox+1;
		minX = maxX - hitbox+1;
		maxY += hitbox - 6;
		engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
	}
	if (playerBehaviourRef->getCurrentDirection() == 2) {
		minY += offset;
		maxY = minY + hitbox;
		maxX += hitbox - 6;
		engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
		//std::cout << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;
	}
	if (playerBehaviourRef->getCurrentDirection() == 3) {
		minX -= (offset + hitbox) ;
		maxX = minX + hitbox ;
		maxY += hitbox - 6;
		engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
		//std::cout << "	// MIN:  " << minX << " , " << minY << " || MAX: " << maxX<< ", " << maxY<< std::endl;
	}

	// check for edgecrashes

	int windowX = 512;
	int windowY = 544;


	if (this->go->horizontalPosition < this->windowCollider->border) {
		this->go->Receive(WALLCRASH);
	}
	else if (this->go->horizontalPosition > this->windowCollider->x - this->windowCollider->border - this->windowCollider->playerWidth) {
		this->go->Receive(WALLCRASH);
	}
	else if (this->go->verticalPosition < (this->windowCollider->header + this->windowCollider->border)) {
		this->go->Receive(WALLCRASH);
	}
	else if (this->go->verticalPosition > this->windowCollider->y - (this->windowCollider->border + this->windowCollider->playerWidth)) {
		this->go->Receive(WALLCRASH);
	}

	// wall collision
	int cellSize = 8;
	int minPosX = minX / cellSize;
	int minPosY = minY / cellSize;
	int maxPosX = maxX/ cellSize;
	int maxPosY = maxY / cellSize;

	if (minPosX < 1) { minPosX = 1; }
	if (minPosY < 8) { minPosY = 8; }
	if (minPosX > 62) { minPosX = 62; }
	if (minPosY > 66) { minPosY = 66; }

	if (maxPosX < 1) { maxPosX = 1; }
	if (maxPosY < 8) { maxPosY = 8; }
	if (maxPosX > 62) { maxPosX = 62; }
	if (maxPosY > 66) { maxPosY = 66; }
	
	//std::cout << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;

	for (int a = minPosX; a <= maxPosX; a++) {
		for (int b = minPosY; b <= maxPosY; b++) {
			if (uniGridref->grid[a][b].state.second != EMPTY) {
				std::cout << "									HIT" << std::endl;
				this->go->Receive(WALLCRASH);
			}
		}
	}
}

void BoxCollideComponent::InsertCurrentCell(UniGridCell cell) {
	currentCellLocation.push_back(cell);
}

void WindowCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<GameObject*>* coll_cycles, int x, int y) {
	Component::Create(engine, go, game_objects);
	this->coll_cycles = coll_cycles;
	
	this->x = x;
	this->y = y;
	//std::cout << "create in window " << std::endl;
}

void WindowCollideComponent::Update(float dt)
{
	int border = 8;
	int playerWidth = 16;
	int header = 64;
	// take game objects x
	WindowCollideComponent* windowCollider = this->go->GetComponent<WindowCollideComponent*>();

	
	for (int i = 0; i < coll_cycles->size(); i++) {
		GameObject* go0 = coll_cycles->at(i);

		bool intersection = false;
		//std::cout << go0->horizontalPosition <<" , " << go0->verticalPosition  << ", " <<  windowCollider->y - 8 - header << std::endl;
		if (go0->horizontalPosition < border || go0->horizontalPosition > windowCollider->x - border - playerWidth ||
			go0->verticalPosition < border || go0->verticalPosition > windowCollider->y - header - (border * 2)) {
			intersection = true;
		}
		if (intersection == true) {
			//go0->Receive(WALLCRASH);
		}

	}
}