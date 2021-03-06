#include "unigrid.h"
#include "player.h"


void UniGridCell::Init(Vector2D min, Vector2D max) {
	this->minPos = min;
	this->maxPos = max;
	int state = 0;  //0 = empty

}

void UniGridCell::Update(double dt)
{
	//somehow tell circlecollide balls to check
}



void UniGrid::Init(int cellSize, std::vector<GameObject*>* lightcycles, int windowX, int windowY) {
	SDL_Log("UniGrid::Init");

	//this->coll_objects = coll_objects;
	this->lightcycles = lightcycles;
	//this->grid_coll_objects = grid_coll_objects;

	int endX = windowX;
	int endY = windowY;

	// starts at 8 because header
	int rowCounter = 0, colCounter = 8;
	int endRow = windowX / cellSize;
	int endCol = windowY / cellSize;

	int minPosX = 0, minPosY = 64;
	int maxPosX = 0 + cellSize, maxPosY = 64 + cellSize;

	//std::cout << "endCol: " << endCol  << std::endl;
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
			currentCell.state = EMPTY;
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
	//CheckCollisions();

	// clear all cells
	for (int i = 0; i < uniGrid->grid.size(); i++) {
		for (int j = 0; j < uniGrid->grid[i].size(); j++) {
			uniGrid->grid[i][j].cyclesInCell.clear();
		}
	}
	// clear hash and objectindex
	for (auto it = uniGrid->hashTable.begin(); it != uniGrid->hashTable.end(); it++) {
		it->second.clear();
		//std::cout << " == Cleared " << it->first.first << ",  " << it->first.second << std::endl;
	}
	for (auto it = uniGrid->objectIndex.begin(); it != uniGrid->objectIndex.end(); it++) {
		it->second = nullptr;
	}

	// clear collision pool
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


	//go through all the cycles, put them in corresponding cells
	for (int i = 0; i < lightcycles->size(); i++) {
		GameObject* currentCycle = lightcycles->at(i);
		int cycleWidth = 14;
		int cycleHeight = 32;
		int positionWidth = 6;

		//floor used to make it int - determines which row and column by dividing cellsize
		int minPosX = floor(currentCycle->horizontalPosition / uniGrid->cellSize);
		int minPosY = floor(currentCycle->verticalPosition/ uniGrid->cellSize);
		int maxPosX = floor((currentCycle->horizontalPosition + cycleWidth) / uniGrid->cellSize);
		int maxPosY = floor((currentCycle->verticalPosition + cycleHeight) / uniGrid->cellSize);

		// leave a border around edge as a "wall"
		if (minPosX < 0) { minPosX = 0; }
		if (minPosY < 0) { minPosY = 0; }
		if (maxPosX > 63) { maxPosX = 63; }
		if (maxPosY > 59) { maxPosY = 59; }

		//if (minPosX <= -1) {
		//	minPosX = 0;
			//std::cout << "	--minX corrected" << std::endl; 
		//}
		//if (minPosY <= -1) {
		//	minPosY = 0;
		//	//std::cout << "	--minY corrected" << std::endl; 
		//}
		//if (maxPosX >= 20) {
		//	maxPosX = 19;
		//	//std::cout << "	++maxX corrected" << std::endl; 
		//}
		//if (maxPosY >= 15) {
		//	maxPosY = 14;
		//	//std::cout << "	++maxY corrected" << std::endl; 
		//}

		//std::cout << "MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;
		//if (minPosX == 1) {
		//	//std::cout << "min = 1  " << std::endl;
		//}
		//if (minPosY == 1) {
		//	//std::cout << "min = 1  " << std::endl;
		//}

		for (int a = minPosX; a <= maxPosX; a++) {
			for (int b = minPosY; b <= maxPosY; b++) {
				//currentBall->GetComponent<CircleCollideComponent*>()->InsertCurrentCell(uniGrid->grid[a][b]);

				uniGrid->grid[a][b].cyclesInCell.push_back(currentCycle);

				//update hash table
				std::pair<int, int> currentHashCellCoord;
				currentHashCellCoord.first = a;
				currentHashCellCoord.second = b;
				uniGrid->hashTable[currentHashCellCoord].push_back(currentCycle);

				// update objectindex
				uniGrid->objectIndex[currentHashCellCoord] = currentCycle;

				if (uniGrid->grid[a][b].cyclesInCell.size() > 1) {
					//std::cout << "pushing to ->  " << a << " , " << b << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;
				}

				if (uniGrid->hashTable[currentHashCellCoord].size() > 1) {
					//std::cout << "	hash X/Y:   "  << currentHashCellCoord.first << ", " << currentHashCellCoord.second << std::endl;
				}
			}
		}
	}

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
		int positionWidth = 6;

		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getGear() == 1) {
			positionWidth = 8;
		}
		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getGear() == 2) {
			positionWidth = 16;
		}

		int minPosX = currentCycle->horizontalPosition / uniGrid->cellSize;
		int minPosY = currentCycle->verticalPosition / uniGrid->cellSize;
		int maxPosX = (currentCycle->horizontalPosition) / uniGrid->cellSize;
		int maxPosY = (currentCycle->verticalPosition) / uniGrid->cellSize;

		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 0) {
			minPosY = (currentCycle->verticalPosition - (positionWidth)) / uniGrid->cellSize;
		}
		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 1) {
			maxPosX = (currentCycle->horizontalPosition + positionWidth) / uniGrid->cellSize;
		}
		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 2) {
			maxPosY = (currentCycle->verticalPosition + positionWidth) / uniGrid->cellSize;
			//std::cout << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;
		}
		if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 3) {
			minPosX = (currentCycle->horizontalPosition  - positionWidth) / uniGrid->cellSize;
			//maxPosX = (currentCycle->horizontalPosition + (positionWidth)) / uniGrid->cellSize;
			//std::cout << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY << std::endl;

		}
		//std::cout << "	// MIN:  " << minPosX << " , " << minPosY << " || MAX: " << maxPosX << ", " << maxPosY<< std::endl;
		// leave a border around edge as a "wall"
		if (minPosX < 1) { minPosX = 1; }
		if (minPosY < 1) { minPosY = 1; }
		if (maxPosX > 62) { maxPosX = 62; }
		if (maxPosY > 58) { maxPosY = 58; }
		

		for (int a = minPosX; a <= maxPosX; a++) {
			for (int b = minPosY; b <= maxPosY; b++) {
				//std::cout << currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() << ", " << currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() << std::endl;
				if (currentCycle->getID() == "player") {

					if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 0) {
						uniGrid->grid[a][b].state = VERTICAL; //vertical
						
						// big brain moment
						// only check corners for the closest cell behind lightcycle
						if (a == maxPosX && b == maxPosY) {
							if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() == 1) {
								//bot left
								uniGrid->grid[a][b].state = BOTRIGHT;
								//std::cout << "BOT RIGHT" << std::endl;
							}
							if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() == 3) {
								//bot right
								uniGrid->grid[a][b].state = BOTLEFT;
								//std::cout << "BOT LEFT" << std::endl;
							}
						}
					}

					if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 2) {
						uniGrid->grid[a][b].state = VERTICAL; //vertical

						if (a == minPosX && b == minPosY) {
							if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() == 1) {
								//bot left
								uniGrid->grid[a][b].state = TOPRIGHT;
								//std::cout << "TOPRIGHT = " << uniGrid->grid[a][b].state << std::endl;
							}
							if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() == 3) {
								//bot right
								uniGrid->grid[a][b].state = TOPLEFT;
								//std::cout << "TOPLEFT" << std::endl;
							}
						}

					}

					if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 1) {
						uniGrid->grid[a][b].state = HORIZONTAL; // horizontal
						
						if (a == minPosX && b == minPosY) {
							if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() == 0) {
								//bot right
								uniGrid->grid[a][b].state = TOPLEFT;
								//std::cout << "TOPLEFT" << std::endl;
							}
							if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() == 2) {
								//top left
								uniGrid->grid[a][b].state = BOTLEFT;
								//std::cout << "BOTLEFT" << std::endl;
							}
						}
					}

					if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getCurrentDirection() == 3) {
						uniGrid->grid[a][b].state = HORIZONTAL; // horizontal

						if (a == maxPosX && b == minPosY) {
							if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() == 0) {
								//bot left
								uniGrid->grid[a][b].state = TOPRIGHT;
								//std::cout << "TOPRIGHT" << std::endl;
							}
							if (currentCycle->GetComponent<PlayerBehaviourComponent*>()->getPrevDirection() == 2) {
								//top right
								uniGrid->grid[a][b].state = BOTRIGHT;
								//std::cout << "BOTRIGHT" << std::endl;
							}
						}
					}
					

				}
			}
		}
	}
}

void UniGrid::CheckCollisions() {
	// check for collisions

	int windowX = 512;
	int windowY = 544;
	int spriteWidth = 16;
	int header = 64;
	int border = 8;

	for (int i = 0; i < lightcycles->size(); i++) {
		GameObject* currentCycle = lightcycles->at(i);

		//for (int i = 0; i < this->grid.size(); i++) {
		//	for (int j = 0; j < this->grid[i].size(); j++) {

		//		if (currentCycle->horizontalPosition < border) {
		//			currentCycle->Receive(WALLCRASH);
		//		}
		//		if (currentCycle->horizontalPosition > windowX - (spriteWidth - border)) {
		//			currentCycle->Receive(WALLCRASH);
		//		}
		//		if (currentCycle->verticalPosition < border) {
		//			currentCycle->Receive(WALLCRASH);
		//		}
		//		if (currentCycle->verticalPosition > windowY - (spriteWidth - border)) {
		//			currentCycle->Receive(WALLCRASH);
		//		}
		//	}
		//}
	}
}


