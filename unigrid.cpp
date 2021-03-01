#include "unigrid.h"
#include "player.h"


void UniGridCell::Init(Vector2D min, Vector2D max) {
	this->minPos = min;
	this->maxPos = max;
	int state = 0;  //0 = empty, 1 = player wall, 2 = enemy wall
}

void UniGridCell::Update(double dt)
{
	//somehow tell circlecollide balls to check
}



void UniGrid::Init(int cellSize, std::vector<GameObject*>* lightcycles, ObjectPool<GameObject>* grid_coll_objects, int windowX, int windowY) {
	SDL_Log("UniGrid::Init");

	//this->coll_objects = coll_objects;
	this->lightcycles = lightcycles;
	this->grid_coll_objects = grid_coll_objects;

	int endX = windowX;
	int endY = windowY;

	int rowCounter = 0, colCounter = 0;
	int endRow = windowX / cellSize;
	int endCol = windowY / cellSize;

	int minPosX = 0, minPosY = 0;
	int maxPosX = 0 + cellSize, maxPosY = 0 + cellSize;

	std::pair<int, int> currentCellCoord;

	for (rowCounter; rowCounter < endRow; rowCounter++) {
		//std::cout << "Current ROW: " << rowCounter << ",  " << maxPosX << std::endl;
		colCounter = 0;
		std::vector<UniGridCell> currentcol;
		for (colCounter; colCounter < endCol; colCounter++) {
			//std::cout << "Current Col: " << colCounter << ", " << maxPosY << std::endl;

			Vector2D currentMinPos = Vector2D(minPosX, minPosY);
			Vector2D currentMaxPos = Vector2D(maxPosX, maxPosY);

			UniGridCell currentCell;
			currentCell.Init(currentMinPos, currentMaxPos);

			currentcol.push_back(currentCell);

			// set hashtable coordinates
			currentCellCoord.first = rowCounter;
			currentCellCoord.second = colCounter;
			hashTable[currentCellCoord];
			//std::cout << "currentcellcoord inserted --" << currentCellCoord.first << ",  " << currentCellCoord.second << std::endl;
			// go down a cell
			minPosY = maxPosY;
			maxPosY += cellSize;

			//end of column, reset y positions
			if (minPosY >= endY) {
				//std::cout << "Resetting, Y =  " << maxPosY << std::endl;
				grid.push_back(currentcol);

				minPosY = 0;
				maxPosY = 0 + cellSize;
			}
		}
		// go to the next row
		minPosX = maxPosX;
		maxPosX += cellSize;
	}
	std::cout << "Ended Grid Formation " << std::endl;
}

void UniGrid::Update(double dt)
{
	//std::cout << "Start Frame " << std::endl;
	UniGrid* uniGrid = this;
	//std::cout << "Unigrid ->  " << uniGrid->grid.size() << std::endl;

	UpdateState(dt);
	// clear all cells
	//for (int i = 0; i < uniGrid->grid.size(); i++) {
	//	for (int j = 0; j < uniGrid->grid[i].size(); j++) {
	//		uniGrid->grid[i][j].ballsInCell.clear();
	//	}
	//}
	//// clear hash and objectindex
	//for (auto it = uniGrid->hashTable.begin(); it != uniGrid->hashTable.end(); it++) {
	//	it->second.clear();
	//	//std::cout << " == Cleared " << it->first.first << ",  " << it->first.second << std::endl;
	//}
	//for (auto it = uniGrid->objectIndex.begin(); it != uniGrid->objectIndex.end(); it++) {
	//	it->second = nullptr;
	//}

	//if (grid_coll_objects->pool.empty()) {
	//	//std::cout << "shit " << std::endl;
	//}
	//else {
	//	grid_coll_objects->pool.clear();
	//}


	//if (grid_coll_objects->pool.empty() != true) {
	//	grid_coll_objects->pool.clear();
	//	//std::cout << "	gridcoll cleared - size "<< grid_coll_objects->pool.size() << std::endl;
	//}


	//go through all the balls, put them in corresponding cells
	//for (int i = 0; i < coll_objects->pool.size(); i++) {
	//	GameObject* currentBall = coll_objects->pool[i];
	//	int ballDiameter = 32;

		// floor used to make it int - determines which row and column by dividing cellsize
		//int minPosX = floor(currentBall->position.x / uniGrid->cellSize);
		//int minPosY = floor(currentBall->position.y / uniGrid->cellSize);
		//int maxPosX = floor((currentBall->position.x + ballDiameter) / uniGrid->cellSize);
		//int maxPosY = floor((currentBall->position.y + ballDiameter) / uniGrid->cellSize);

	//	if (minPosX <= -1) {
	//		minPosX = 0;
	//		//std::cout << "	--minX corrected" << std::endl; 
	//	}
	//	if (minPosY <= -1) {
	//		minPosY = 0;
	//		//std::cout << "	--minY corrected" << std::endl; 
	//	}
	//	if (maxPosX >= 20) {
	//		maxPosX = 19;
	//		//std::cout << "	++maxX corrected" << std::endl; 
	//	}
	//	if (maxPosY >= 15) {
	//		maxPosY = 14;
	//		//std::cout << "	++maxY corrected" << std::endl; 
	//	}
	//	//std::cout << "MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;
	//	if (minPosX == 1) {
	//		//std::cout << "min = 1  " << std::endl;
	//	}
	//	if (minPosY == 1) {
	//		//std::cout << "min = 1  " << std::endl;
	//	}

	//	for (int a = minPosX; a <= maxPosX; a++) {
	//		for (int b = minPosY; b <= maxPosY; b++) {
	//			//currentBall->GetComponent<CircleCollideComponent*>()->InsertCurrentCell(uniGrid->grid[a][b]);

	//			uniGrid->grid[a][b].ballsInCell.push_back(currentBall);

	//			//update hash table
	//			std::pair<int, int> currentHashCellCoord;
	//			currentHashCellCoord.first = a;
	//			currentHashCellCoord.second = b;
	//			uniGrid->hashTable[currentHashCellCoord].push_back(currentBall);

	//			// update objectindex
	//			uniGrid->objectIndex[currentHashCellCoord] = currentBall;

	//			if (uniGrid->grid[a][b].ballsInCell.size() > 1) {
	//				//std::cout << "pushing to ->  " << a << " , " << b << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;
	//			}

	//			if (uniGrid->hashTable[currentHashCellCoord].size() > 1) {
	//				//std::cout << "	hash X/Y:   "  << currentHashCellCoord.first << ", " << currentHashCellCoord.second << std::endl;
	//			}
	//		}
	//	}
	//}

	//// go through all cells, put any cell that has 2+ balls into cells to check
	//for (int i = 0; i < uniGrid->grid.size(); i++) {
	//	for (int j = 0; j < uniGrid->grid[i].size(); j++) {
	//		// only look for cells with multiple balls
	//		if (uniGrid->grid[i][j].ballsInCell.size() > 1) {
	//			for (int k = 0; k < uniGrid->grid[i][j].ballsInCell.size(); k++) {
	//				//std::cout << "	Inserted ball to check" << std::endl;
	//				grid_coll_objects->pool.push_back(uniGrid->grid[i][j].ballsInCell[k]);
	//			}
	//		}
	//	}
	//}

}

void UniGrid::UpdateState(double dt)
{
	UniGrid* uniGrid = this;

	for (int i = 0; i < lightcycles->size(); i++) {
		GameObject* currentCycle = lightcycles->at(i);

		//std::cout << lightcycles->at(i) << std::endl;

		int cycleWidth = 14;
		int cycleHeight = 32;
		int positionWidth = 7;

		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getGear() == 1) {
			positionWidth = 8;
		}
		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getGear() == 2) {
			positionWidth = 16;
		}

		int minPosX = currentCycle->horizontalPosition / uniGrid->cellSize;
		int minPosY = currentCycle->verticalPosition / uniGrid->cellSize;
		int maxPosX = (currentCycle->horizontalPosition + positionWidth) / uniGrid->cellSize;
		int maxPosY = (currentCycle->verticalPosition + positionWidth) / uniGrid->cellSize;

		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 0) {
			maxPosX = (currentCycle->horizontalPosition) / uniGrid->cellSize;
			maxPosY = (currentCycle->verticalPosition + positionWidth) / uniGrid->cellSize;
		}
		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 1) {
			maxPosX = (currentCycle->horizontalPosition + positionWidth) / uniGrid->cellSize;
			maxPosY = (currentCycle->verticalPosition) / uniGrid->cellSize;
		}
		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 2) {
			maxPosX = (currentCycle->horizontalPosition) / uniGrid->cellSize;
			maxPosY = (currentCycle->verticalPosition + positionWidth) / uniGrid->cellSize;
		}
		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 3) {
			maxPosX = (currentCycle->horizontalPosition + positionWidth) / uniGrid->cellSize;
			maxPosY = (currentCycle->verticalPosition) / uniGrid->cellSize;
		}
		//std::cout << "	// MIN:  " << currentCycle->verticalPosition << " / " << uniGrid->cellSize << " || MAX: " << currentCycle->verticalPosition + positionWidth << "/ " << uniGrid->cellSize << std::endl;

		for (int a = minPosX; a <= maxPosX; a++) {
			for (int b = minPosY; b <= maxPosY; b++) {
				//std::cout << "pushing to ->  " << a << " , " << b << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;
				if (currentCycle->getID() == "player") {
					uniGrid->grid[a][b].state = 1;
				}
			}
		}

	}
}


