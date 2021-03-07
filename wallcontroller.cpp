#include "wallcontroller.h"

void WallController::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, UniGrid* unigridRef) {
	this->unigridRef = unigridRef;
	this->go = go;
	this->engine = engine;
	this->game_objects = game_objects;

}
void WallController::Update(float dt) {
	drawWalls();
}

void WallController::drawWalls() {

	//Lightwall* lightwall = new Lightwall();
	//LightwallBehaviourComponent* player_lightwall_behaviour = new LightwallBehaviourComponent();
	//player_lightwall_behaviour->Create(engine, lightwall, this->game_objects);

	//std::vector<const char*> player_lightwallSpriteNames;
	//player_lightwallSpriteNames.push_back("data/VertLightwallPlayer.bmp");
	//player_lightwallSpriteNames.push_back("data/HorizLightwallPlayer.bmp");

	//LightwallRenderComponent* player_lightwall_render = new LightwallRenderComponent();
	//player_lightwall_render->Create(engine, lightwall, this->game_objects, player_lightwallSpriteNames);
	//lightwall->Create();
	//lightwall->AddComponent(player_lightwall_behaviour);
	//lightwall->AddComponent(player_lightwall_render);

	for (int i = 0; i < this->unigridRef->grid.size(); i++) {
		for (int j = 0; j < this->unigridRef->grid[i].size(); j++) {

			if (this->unigridRef->grid[i][j].state.second == VERTICAL) {
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, 51, 153, 255);
			}

			if (this->unigridRef->grid[i][j].state.second == HORIZONTAL) {
				//lightwall->Init(go->horizontalPosition + 2, go->verticalPosition, 1);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, 51, 153, 255);
			}
			if (this->unigridRef->grid[i][j].state.second == TOPLEFT) {
				//std::cout << "shit" << std::endl;
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, 51, 153, 255);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, 51, 153, 255);
			}

			if (this->unigridRef->grid[i][j].state.second == TOPRIGHT) {
				//std::cout << "shitTOPRIGHT" << std::endl;
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, 51, 153, 255);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, 51, 153, 255);
			}

			if (this->unigridRef->grid[i][j].state.second == BOTRIGHT) {
				//std::cout << "shitBOTRIGHT" << std::endl;
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y - 2);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, 51, 153, 255);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, 51, 153, 255);
			}
			if (this->unigridRef->grid[i][j].state.second == BOTLEFT) {
				//std::cout << "shit" << std::endl;
				Vector2D vertMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y);
				Vector2D vertMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x - 2, this->unigridRef->grid[i][j].maxPos.y - 2);
				Vector2D horizMinWallPos = Vector2D(this->unigridRef->grid[i][j].minPos.x + 2, this->unigridRef->grid[i][j].minPos.y + 2);
				Vector2D horizMaxWallPos = Vector2D(this->unigridRef->grid[i][j].maxPos.x, this->unigridRef->grid[i][j].maxPos.y - 2);
				engine->fillRect(vertMinWallPos, vertMaxWallPos, 51, 153, 255);
				engine->fillRect(horizMinWallPos, horizMaxWallPos, 51, 153, 255);
			}
		}
	}
}