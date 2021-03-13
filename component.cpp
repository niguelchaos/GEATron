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



void CollideComponent::Create(AvancezLib* engine, GameObject * go, std::set<GameObject*> * game_objects, std::vector<GameObject> * coll_objects)
{
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
}


void CollideComponent::Update(float dt)
{

}

void BoxCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<GameObject*>* coll_objects, UniGrid* uniGrid, WindowCollideComponent* windowCollider) {
	Component::Create(engine, go, game_objects);
	int playerHitboxWidth = 8;
	int playerHitboxHeight = 8;

	this->coll_objects = coll_objects;
	this->minX = go->horizontalPosition;
	this->minY = go->verticalPosition;
	this->maxX = minX + playerHitboxWidth;
	this->maxY = minY + playerHitboxHeight;
	this->uniGridref = uniGrid;
	this->windowCollider = windowCollider;

	this->currentCellLocation = {};
	this->currentHashCellLocation = {}; // could this be the object index instead? 
	this->prevHashCellLocation = {}; // could this be the object index instead? 

	std::pair<int, int> prevHashCellCoord;
	prevHashCellCoord.first = this->minX;
	prevHashCellCoord.second = this->minY;
	prevHashCellLocation.push_back(prevHashCellCoord);

	prevHashCellCoord.first = this->maxX;
	prevHashCellCoord.second = this->maxY;
	prevHashCellLocation.push_back(prevHashCellCoord);
}

int BoxCollideComponent::getMinX() { return minX; }
int BoxCollideComponent::getMinY() { return minY; }
int BoxCollideComponent::getMaxX() { return maxX; }
int BoxCollideComponent::getMaxY() { return maxY; }


void BoxCollideComponent::Update(float dt)
{
	// clear currentcellslocation
	currentCellLocation.clear();
	currentHashCellLocation.clear();


	// take game objects x
	BoxCollideComponent* boxCollider = go->GetComponent<BoxCollideComponent*>();
	PlayerBehaviourComponent* playerBehaviourRef = go->GetComponent<PlayerBehaviourComponent*>();

	int hitbox = 7;
	int offset = 9;

	minX = go->horizontalPosition;
	minY = go->verticalPosition;
	maxX = go->horizontalPosition;
	maxY = (go->verticalPosition ); 
		

	if (playerBehaviourRef->getGear() == 1) {
		offset = 16;
	}
	if (playerBehaviourRef->getGear() == 2) {
		hitbox = 8;
		offset = 24; 
	}

	if (playerBehaviourRef->getCurrentDirection() == 0) {
		maxY -= offset;
		minY = maxY - hitbox;
		maxX += hitbox ;
		//engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
		//std::cout << "			|| MIN:  " << minX << " , " << minY << " || MAX: " << maxX << ", " << maxY << std::endl;
	}
	if (playerBehaviourRef->getCurrentDirection() == 1) {
		maxX += offset + hitbox+1;
		minX = maxX - hitbox+1;
		maxY += hitbox;
		//engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
	}
	if (playerBehaviourRef->getCurrentDirection() == 2) {
		minY += offset;
		maxY = minY + hitbox;
		maxX += hitbox ;
		//engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
		//std::cout << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;
	}
	if (playerBehaviourRef->getCurrentDirection() == 3) {
		minX -= (offset + hitbox) ;
		maxX = minX + hitbox ;
		maxY += hitbox ;
		//engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
		//std::cout << "	// MIN:  " << minX << " , " << minY << " || MAX: " << maxX<< ", " << maxY<< std::endl;
	}
	engine->drawCell(Vector2D(minX, minY), Vector2D(maxX, maxY));
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
	
	int prevMinX = prevHashCellLocation[0].first / cellSize;
	int prevMinY = prevHashCellLocation[0].second / cellSize;
	int prevMaxX = prevHashCellLocation[1].first / cellSize;
	int prevMaxY = prevHashCellLocation[1].second / cellSize;

	if (prevMinX < 1) { prevMinX = 1; }
	if (prevMinY < 8) { prevMinY = 8; }
	if (prevMinX > 62) { prevMinX = 62; }
	if (prevMinY > 66) { prevMinY = 66; }

	if (prevMaxX < 1) { prevMaxX = 1; }
	if (prevMaxY < 8) { prevMaxY = 8; }
	if (prevMaxX > 62) { prevMaxX = 62; }
	if (prevMaxY > 66) { prevMaxY = 66; }

	//std::cout << "	// MIN:  " << minX << " , " << minY << " || MAX: " << maxX << ", " << maxY << std::endl;
	//std::cout << "	// prev MIN:  " << prevHashCellLocation[0].first << " , " << prevHashCellLocation[0].second << " || MAX: " << prevHashCellLocation[1].first  << ", " << prevHashCellLocation[1].second  << std::endl;
		// idk why create doesnt work here
	if (prevMinX == 1) {
		std::cout << "prevhashcell = 0" << std::endl;
		prevHashCellLocation.clear();
		std::pair<int, int> prevHashCellCoord;
		prevHashCellCoord.first = this->minX;
		prevHashCellCoord.second = this->minY;
		prevHashCellLocation.push_back(prevHashCellCoord);

		prevHashCellCoord.first = this->maxX;
		prevHashCellCoord.second = this->maxY;
		prevHashCellLocation.push_back(prevHashCellCoord);
	}

	//std::cout << "	prevhashcell " << prevHashCellLocation[0].first / cellSize  << ", " << prevHashCellLocation[0].second / cellSize << std::endl;
	//std::cout << "	prevhashcell " << prevHashCellLocation[1].first / cellSize << ", " << prevHashCellLocation[1].second / cellSize << std::endl;
	// get all objects in those cells
	for (int a = minPosX; a <= maxPosX; a++) {
		for (int b = minPosY; b <= maxPosY; b++) {
			//currentCellLocation.push_back(uniGridref->grid[a][b]);

			// find own location in object index, update current coordinates in the hashmap
			std::pair<int, int> currentHashCellCoord;
			currentHashCellCoord.first = a;
			currentHashCellCoord.second = b;
			currentHashCellLocation.push_back(currentHashCellCoord);

			//std::cout << "	currenthashcell pushed box collide  -- " << currentHashCellCoord.first << ", " << currentHashCellCoord.second << std::endl;
		}
	}

	
	// god this is cursed
	if (this->go->GetComponent<PlayerBehaviourComponent*>()->isRekt() == false) {
		//std::cout << "		not rekt" << std::endl;
		// current maxY < prev minY = UP
		if (maxY < prevHashCellLocation[0].second) {
			for (int a = minPosX; a <= maxPosX; a++) {
				for (int b = minPosY; b < prevMinY; b++) {
					if (uniGridref->grid[a][b].state.second != EMPTY) {
						std::cout << "			UP		HASH		HIT" << std::endl;
						//engine->drawCell(Vector2D(prevHashCellLocation[0].first, prevHashCellLocation[0].second), Vector2D(prevHashCellLocation[1].first, prevHashCellLocation[1].second));
						this->go->Receive(WALLCRASH);
						break;
					}
				}
			}
		}


		// current maxX  > prev maxX = RIGHT
		if (maxX > prevHashCellLocation[1].first) {
			for (int a = prevMaxX; a < minPosX; a++) {
				for (int b = minPosY; b < maxPosY; b++) {
					if (uniGridref->grid[a][b].state.second != EMPTY) {
						std::cout << "			RIGHT		HASH		HIT" << std::endl;
						this->go->Receive(WALLCRASH);
						break;
					}
				}
			}
		}
		// //current maxX < prevMaxX = LEFT
		if (maxX < prevHashCellLocation[1].first) {
			for (int a = minPosX; a < prevMinX; a++) {
				for (int b = minPosY; b <= maxPosY; b++) {
					if (uniGridref->grid[a][b].state.second != EMPTY) {
						std::cout << "			LEFT		HASH		HIT" << std::endl;
						this->go->Receive(WALLCRASH);
						break;
					}
				}
			}
		}
		// current maxY > prev maxY = DOWN
		if (minY > prevHashCellLocation[1].second) {
			for (int a = minPosX; a < maxPosX; a++) {
				for (int b = prevMaxY; b < minPosY; b++) {
					if (uniGridref->grid[a][b].state.second != EMPTY) {
						std::cout << "			DOWN		HASH		HIT" << std::endl;
						this->go->Receive(WALLCRASH);
						break;
					}
				}
			}
		}
	}

	engine->drawCell(Vector2D((prevHashCellLocation[0].first), (prevHashCellLocation[0].second)), Vector2D((prevHashCellLocation[1].first), (prevHashCellLocation[1].second)));
	//// original ////
	for (int a = minPosX; a <= maxPosX; a++) {
		for (int b = minPosY; b <= maxPosY; b++) {
			if (uniGridref->grid[a][b].state.second != EMPTY) {
				std::cout << "									HIT" << std::endl;
				this->go->Receive(WALLCRASH);
			}
		}
	}


	// remember for next frame comparison
	prevHashCellLocation.clear();

	std::pair<int, int> prevHashCellCoord;
	prevHashCellCoord.first = minX;
	prevHashCellCoord.second = minY;
	prevHashCellLocation.push_back(prevHashCellCoord);

	prevHashCellCoord.first = maxX;
	prevHashCellCoord.second = maxY;
	prevHashCellLocation.push_back(prevHashCellCoord);

	//std::cout << "	prevhashcell pushed box collide  -- " << prevHashCellCoord.first << ", " << prevHashCellCoord.second << std::endl;
		
	
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