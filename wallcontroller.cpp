#include "wallcontroller.h"

void WallController::Create(AvancezLib* engine, std::set<GameObject*>* game_objects, UniGrid* unigridRef) {
	this->unigridRef = unigridRef;
	// is not gameobject
	//this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;

}
void WallController::Update(float dt) {
	drawWalls();
}

void WallController::drawWalls() {
	// colors
	int p1r = 51;	int p1g = 153;	int p1b = 255;
	int p2r = 255;	int p2g = 178;	int p2b = 102;

	for (int i = 0; i < this->unigridRef->grid.size(); i++) {
		for (int j = 0; j < this->unigridRef->grid[i].size(); j++) {

			if (this->unigridRef->grid[i][j].state.second == VERTICAL) {
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, p1r, p1g, p1b);

				if (this->unigridRef->grid[i][j].state.first != 1) {
					engine->fillRect(vertMinWallPos, vertMaxWallPos, p2r, p2g, p2b);
				}
			}

			if (this->unigridRef->grid[i][j].state.second == HORIZONTAL) {
				//lightwall->Init(go->horizontalPosition + 2, go->verticalPosition, 1);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, p1r, p1g, p1b);

				if (this->unigridRef->grid[i][j].state.first != 1) {
					engine->fillRect(horizMinWallPos, horizMaxWallPos, p2r, p2g, p2b);
				}
			}
			if (this->unigridRef->grid[i][j].state.second == TOPLEFT) {
				//std::cout << "shit" << std::endl;
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, p1r, p1g, p1b);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, p1r, p1g, p1b);

				if (this->unigridRef->grid[i][j].state.first != 1) {
					engine->fillRect(vertMinWallPos, vertMaxWallPos, p2r, p2g, p2b);
					engine->fillRect(horizMinWallPos, horizMaxWallPos, p2r, p2g, p2b);
				}
			}

			if (this->unigridRef->grid[i][j].state.second == TOPRIGHT) {
				//std::cout << "shitTOPRIGHT" << std::endl;
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, p1r, p1g, p1b);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, p1r, p1g, p1b);

				if (this->unigridRef->grid[i][j].state.first != 1) {
					engine->fillRect(vertMinWallPos, vertMaxWallPos, p2r, p2g, p2b);
					engine->fillRect(horizMinWallPos, horizMaxWallPos, p2r, p2g, p2b);
				}
			}

			if (this->unigridRef->grid[i][j].state.second == BOTRIGHT) {
				//std::cout << "shitBOTRIGHT" << std::endl;
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y - 2);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, p1r, p1g, p1b);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, p1r, p1g, p1b);

				if (this->unigridRef->grid[i][j].state.first != 1) {
					engine->fillRect(vertMinWallPos, vertMaxWallPos, p2r, p2g, p2b);
					engine->fillRect(horizMinWallPos, horizMaxWallPos, p2r, p2g, p2b);
				}
			}
			if (this->unigridRef->grid[i][j].state.second == BOTLEFT) {
				//std::cout << "shit" << std::endl;
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y - 2);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, p1r, p1g, p1b);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, p1r, p1g, p1b);

				if (this->unigridRef->grid[i][j].state.first != 1) {
					engine->fillRect(vertMinWallPos, vertMaxWallPos, p2r, p2g, p2b);
					engine->fillRect(horizMinWallPos, horizMaxWallPos, p2r, p2g, p2b);
				}
			}
		}
	}
}