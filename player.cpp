#include "player.h"

void PlayerBehaviourComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects)
{
	Component::Create(engine, go, game_objects);
	this->playerNum = 1;
	this->startX = 288;
	this->startY = 512;
	this->currentDirection = 0;
}

void Player2BehaviourComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects) 
{
	Component::Create(engine, go, game_objects);
	this->playerNum = 2;
	this->startX = 160;
	this->startY = 128;
	this->currentDirection = 2;
}

void PlayerBehaviourComponent::Init()
{
	this->go->horizontalPosition = startX;
	this->go->verticalPosition = startY;
	prevPosX = this->go->horizontalPosition;
	prevPosY = this->go->verticalPosition;

	//time_fire_pressed = -10000.f;
	//time_turn_pressed = -10000.f;
}

void PlayerBehaviourComponent::Update(float dt)
{
	AvancezLib::KeyStatus keys;
	int windowX = 512;
	int windowY = 544;
	int spriteWidth = 16;
	int header = 64;
	int border = 8;
	timer += dt;

	if (paused == true) {
		timer = 0;
	}

	if (timer > TIMER_CD) {

		if (!isRekt()) {
			prevDirection = currentDirection;
			prevPosX = go->horizontalPosition;
			prevPosY = go->verticalPosition;

			//if (engine->isSoundPlaying() == 0) {
			//	engine->LoadSound("data/cyclespeedingphased.wav");
			//	engine->PlaySound(-1);
			//	std::cout << "playing sound" << std::endl;
			//}

			if (currentDirection == 0) { go->verticalPosition += -speed; }
			if (currentDirection == 1) { go->horizontalPosition += speed; }
			if (currentDirection == 2) { go->verticalPosition += speed; }
			if (currentDirection == 3) { go->horizontalPosition += -speed; }


			// LIMITS FOR PLAYER TO STAY IN FRAME
			if (go->horizontalPosition > (windowX - (spriteWidth - border))) { go->horizontalPosition = windowX - (spriteWidth - border); }
			if (go->horizontalPosition < 0) { go->horizontalPosition = 0; }
			if (go->verticalPosition > (windowY - (spriteWidth - border))) { go->verticalPosition = windowY - (spriteWidth - border); }
			if (go->verticalPosition < header ) { go->verticalPosition = header; }


			//std::cout << "	prev Pos = " << prevPosX << " -- " << prevPosY << std::endl;
			//std::cout << "current Pos = " << go->horizontalPosition << " -- " <<  go->verticalPosition << std::endl;
			

			engine->getKeyStatus(keys);

			if (keys.right) {
				if (prevKeyRight == false) {
					ChangeDirection(RIGHT);
				}
				prevKeyRight = keys.right;
			}
			if (keys.left) {
				if (prevKeyLeft == false) {
					ChangeDirection(LEFT);
				}
				prevKeyLeft = keys.left;
			}
			if (!keys.right) { prevKeyRight = keys.right; }
			if (!keys.left) { prevKeyLeft = keys.left; }

			if (keys.up == true) {
				if (prevKeyUp == false) {
					ChangeSpeed(ACCELERATE);
				}
				prevKeyUp = keys.up;
			}
			if (keys.down == true) {
				if (prevKeyDown == false) {
					ChangeSpeed(DECELERATE);
				}
				prevKeyDown = keys.down;
			}

			if (!keys.up) { prevKeyUp = keys.up; }
			if (!keys.down) { prevKeyDown = keys.down; }
			
		}

		timer = 0;
	}

}

void Player2BehaviourComponent::Update(float dt)
{
	AvancezLib::KeyStatus keys;
	int windowX = 512;
	int windowY = 544;
	int spriteWidth = 16;
	int header = 64;
	int border = 8;
	timer += dt;

	if (paused == true) {
		timer = 0;
	}

	if (timer > TIMER_CD) {

		if (!isRekt()) {
			//go->verticalPosition = go->verticalPosition + header;
			//go->horizontalPosition = go->horizontalPosition;
			prevDirection = currentDirection;
			prevPosX = go->horizontalPosition;
			prevPosY = go->verticalPosition;

			
			engine->PlaySound(0, 2);


			if (currentDirection == 0) { go->verticalPosition += -speed; }
			if (currentDirection == 1) { go->horizontalPosition += speed; }
			if (currentDirection == 2) { go->verticalPosition += speed; }
			if (currentDirection == 3) { go->horizontalPosition += -speed; }


			//go->verticalPosition = floor(go->verticalPosition);
			//go->horizontalPosition = floor(go->horizontalPosition);
			//if (prevDirection != currentDirection) {
				//std::cout << "waw" << std::endl;

			//}
			// LIMITS FOR PLAYER TO STAY IN FRAME
			if (go->horizontalPosition > (windowX - (spriteWidth - border))) { go->horizontalPosition = windowX - (spriteWidth - border); }
			if (go->horizontalPosition < 0) { go->horizontalPosition = 0; }
			if (go->verticalPosition > (windowY - (spriteWidth - border))) { go->verticalPosition = windowY - (spriteWidth - border); }
			if (go->verticalPosition < header) { go->verticalPosition = header; }


			//std::cout << "	prev Pos = " << prevPosX << " -- " << prevPosY << std::endl;
			//std::cout << "current Pos = " << go->horizontalPosition << " -- " << go->verticalPosition << std::endl;


			engine->getKeyStatus(keys);

			//////////////////// p2 ////////////////////
			//std::cout << playerNum << std::endl;
			if (keys.d) {
				if (prevKeyd == false) {
					ChangeDirection(RIGHT);
				}
				prevKeyd = keys.d;
			}
			if (keys.a) {
				if (prevKeya == false) {
					ChangeDirection(LEFT);
				}
				prevKeya = keys.a;
			}
			if (!keys.d) { prevKeyd = keys.d; }
			if (!keys.a) { prevKeya = keys.a; }

			if (keys.w == true) {
				if (prevKeyw == false) {
					ChangeSpeed(ACCELERATE);
				}
				prevKeyw = keys.w;
			}
			if (keys.s == true) {
				if (prevKeys == false) {
					ChangeSpeed(DECELERATE);
				}
				prevKeys = keys.s;
			}

			if (!keys.w) { prevKeyw = keys.w; }
			if (!keys.s) { prevKeys = keys.s; }

		}
		timer = 0;
	}
}

void PlayerBehaviourComponent::Receive(Message m)
{
	if (m == WALLCRASH) {
		rekt = true;
	}
}


void PlayerBehaviourComponent::playRektMp3() {
	//engine->LoadMp3("data/soddingtictac.mp3");
	//engine->PlayMp3(0);
}
//void Player2BehaviourComponent::Receive(Message m)
//{
//	if (m == WALLCRASH) {
//		rekt = true;
//		engine->finishSound();
//		engine->finishMp3();
//		if (engine->isMp3Playing() == 0) {
//			engine->LoadMp3("data/sickcrash.mp3");
//			engine->PlayMp3(0);
//		}
//	}
//}

bool PlayerBehaviourComponent::isRekt() {
	return rekt;
}

void PlayerBehaviourComponent::ChangeDirection(int direction)
{
	//go->horizontalPosition += move;
	//if ((engine->getElapsedTime() - time_turn_pressed) < (TURN_COOLDOWN))
	currentDirection += direction;

	if (currentDirection > 3) {	currentDirection = 0; }
	if (currentDirection < 0) { currentDirection = 3; }

	//std::cout << "current Direction = " << currentDirection << std::endl;
	//time_turn_pressed = engine->getElapsedTime();
}
void PlayerBehaviourComponent::ChangeSpeed(int change)
{
	gear += change;
	if (gear < 0) { gear = 0; }
	if (gear > 2) { gear = 2; }

	if (gear == 0) { speed = 8; }
	if (gear == 1) { speed = 16; }
	if (gear == 2) { speed = 24; }
	//if (gear == 3) { speed = 24; }
	//if (gear == 4) { speed = 24; }

	std::cout << "change: " << gear << "	speed: " << speed << std::endl;
}

int PlayerBehaviourComponent::getGear() {
	return gear;
}

float PlayerBehaviourComponent::getCurrentDirection() {
	return currentDirection;
}

float PlayerBehaviourComponent::getPrevDirection() {
	return prevDirection;
}

void PlayerBehaviourComponent::Pause() {
	paused = true;
	timer = 0;
}

void PlayerRenderComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<const char*> sprite_names, PlayerBehaviourComponent* playerBehaviourRef)
{
	Component::Create(engine, go, game_objects);
	this->playerBehaviourRef = playerBehaviourRef;

	for (int i = 0; i < sprite_names.size(); i++) {
		Sprite* sprite = engine->createSprite(sprite_names[i]);
		sprites.push_back(sprite);
	}
}

void PlayerRenderComponent::Update(float dt)
{
	if (!go->enabled) {
		return;
	}
	int offset = 3;
	int lateralOffset = 32;
	int header = 64; 
	int xPos;
	int yPos;

	xPos = int(go->horizontalPosition);
	yPos = int(go->verticalPosition);

	if (playerBehaviourRef->getCurrentDirection() == 0) {
		xPos = int(go->horizontalPosition - offset);
		yPos = int(go->verticalPosition - lateralOffset);
	}
	if (playerBehaviourRef->getCurrentDirection() == 1) {
		xPos = int(go->horizontalPosition );
		yPos = int(go->verticalPosition - offset);
	}
	if (playerBehaviourRef->getCurrentDirection() == 2) {
		xPos = int(go->horizontalPosition - offset);
		yPos = int(go->verticalPosition);
	}
	if (playerBehaviourRef->getCurrentDirection() == 3) {
		xPos = int(go->horizontalPosition - lateralOffset);
		yPos = int(go->verticalPosition - offset);
	}
	if (isRekt) {
		int expOffset = 12;
		if (playerBehaviourRef->getCurrentDirection() == 0) {
			xPos = int(go->horizontalPosition - expOffset);
			yPos = int(go->verticalPosition);
		}
		if (playerBehaviourRef->getCurrentDirection() == 1) {
			xPos = int(go->horizontalPosition);
			yPos = int(go->verticalPosition - expOffset);
		}
		if (playerBehaviourRef->getCurrentDirection() == 2) {
			xPos = int(go->horizontalPosition - expOffset);
			yPos = int(go->verticalPosition);
		}
		if (playerBehaviourRef->getCurrentDirection() == 3) {
			xPos = int(go->horizontalPosition - lateralOffset);
			yPos = int(go->verticalPosition - expOffset);
		}
		//std::cout << xPos << ", " << yPos << std::endl;
	}

	if (!isRekt) {

		if (playerBehaviourRef->getCurrentDirection() == 0) {
			//std::cout << "showing going up" << std::endl;
			sprites[0]->draw(xPos, yPos);
		}
		if (playerBehaviourRef->getCurrentDirection() == 1) {
			sprites[1]->draw(xPos, yPos);
		}
		if (playerBehaviourRef->getCurrentDirection() == 2) {
			sprites[2]->draw(xPos, yPos);
		}
		if (playerBehaviourRef->getCurrentDirection() == 3) {
			sprites[3]->draw(xPos, yPos);
		}
	}

	if (explosionAnimationFrame >= 15 && explosionAnimationFrame < 20 || 
		explosionAnimationFrame >= 20 && explosionAnimationFrame < 25) {
		sprites[7]->draw(xPos, yPos);
		explosionAnimationFrame++;
	}
	if (explosionAnimationFrame >= 10 && explosionAnimationFrame < 15 || 
		explosionAnimationFrame >= 25 && explosionAnimationFrame < 30) {
		sprites[6]->draw(xPos, yPos);
		explosionAnimationFrame++;
	}
	if (explosionAnimationFrame >= 5 && explosionAnimationFrame < 10 ||
		explosionAnimationFrame >= 30 && explosionAnimationFrame < 35) {
		sprites[5]->draw(xPos, yPos);
		explosionAnimationFrame++;
	}
	if (explosionAnimationFrame > 0 && explosionAnimationFrame < 5 || 
		explosionAnimationFrame >= 35 && explosionAnimationFrame < 40) {
		sprites[4]->draw(xPos, yPos);
		explosionAnimationFrame++;
	}
	if (explosionAnimationFrame > 40) {
		explosionAnimationFrame = 0;
		doneExploding = true;
		//go->enabled = false;

	}

	if (isRekt) {
		explosionAnimationFrame++;
	}

		
}

bool PlayerRenderComponent::isDoneExploding() {
	return doneExploding;
}

void PlayerRenderComponent::Destroy() 
{
	for (int i = 0; i < sprites.size(); i++) {
		if (sprites[i] != NULL)
			sprites[i]->destroy();
		sprites[i] = NULL;
	}
}

void PlayerRenderComponent::Receive(Message m)
{
	if (m == WALLCRASH) {
		isRekt = true;
	}
}

void Player::Init()
{
	SDL_Log("Player::Init");
	GameObject::Init();
	lives = 0;
	id = "player";
}

void Player::Receive(Message m)
{
	if (m == WALLCRASH) {
		//SDL_Log("Player::CRASH!");
		if (enabled == true) {
			RemoveLife();
		}

		this->GetComponent<PlayerBehaviourComponent*>()->Receive(WALLCRASH);
		this->GetComponent<PlayerRenderComponent*>()->Receive(WALLCRASH);

		if (lives < 0 && enabled == true) {
			SDL_Log("Player::REKT");
			Send(GAME_OVER);
		}
	}
}

void Player::RemoveLife()
{
	lives--;
	//SDL_Log("remaining lives %d", lives);
}

