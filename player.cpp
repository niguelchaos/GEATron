#include "player.h"
#include "lightwall.h"

void PlayerBehaviourComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<Lightwall>* lightwall_pool)
{
	Component::Create(engine, go, game_objects);
	this->lightwall_pool = lightwall_pool;
}

void PlayerBehaviourComponent::Init()
{
	go->horizontalPosition = 320;
	go->verticalPosition = 472 - 8;
	prevPosX = go->horizontalPosition;
	prevPosY = go->verticalPosition;

	time_fire_pressed = -10000.f;
	time_turn_pressed = -10000.f;
}

void PlayerBehaviourComponent::Update(float dt)
{
	AvancezLib::KeyStatus keys;
	int windowX = 512;
	int windowY = 480;
	int spriteWidth = 16;
	int border = 8;
	timer += dt;

	if (timer > TIMER_CD) {

		if (currentDirection == 0) { go->verticalPosition += -speed; }
		if (currentDirection == 1) { go->horizontalPosition += speed; }
		if (currentDirection == 2) { go->verticalPosition += speed; }
		if (currentDirection == 3) { go->horizontalPosition += -speed; }

		//go->verticalPosition = floor(go->verticalPosition);
		//go->horizontalPosition = floor(go->horizontalPosition);

		if (go->horizontalPosition > (windowX - spriteWidth)) { go->horizontalPosition = windowX - spriteWidth; }
		if (go->horizontalPosition < border) { go->horizontalPosition = border; }
		if (go->verticalPosition > (windowY - spriteWidth)) { go->verticalPosition = windowY - spriteWidth; }
		if (go->verticalPosition < border) { go->verticalPosition = border; }

		//std::cout << "current Pos = " << go->verticalPosition << " -- " << go->horizontalPosition << std::endl;

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

		Lightwall* lightwall = lightwall_pool->FirstAvailable();
		int centerOffset = 14;
		int sideOffset = 8;
		int heightOffset = 8;

		int wallWidth = 4;
		int wallHeight = 8;

		if (go->horizontalPosition != prevPosX || go->verticalPosition != prevPosY) {
			if (lightwall != NULL)	// wall is NULL is the object pool can not provide an object
			{
				if (currentDirection == 0) {
					lightwall->Init(go->horizontalPosition + 2, go->verticalPosition, currentDirection);
					//lightwall->Init(go->horizontalPosition + centerOffset, go->verticalPosition + heightOffset, currentDirection);
				}
				if (currentDirection == 1) {
					lightwall->Init(go->horizontalPosition, go->verticalPosition + 2, currentDirection);
					//lightwall->Init(go->horizontalPosition + heightOffset + heightOffset, go->verticalPosition + centerOffset, currentDirection);
				}
				if (currentDirection == 2) {
					lightwall->Init(go->horizontalPosition + 2, go->verticalPosition, currentDirection);
					//lightwall->Init(go->horizontalPosition + centerOffset, go->verticalPosition + heightOffset + heightOffset, currentDirection);
				}
				if (currentDirection == 3) {
					lightwall->Init(go->horizontalPosition, go->verticalPosition + 2, currentDirection);
					//lightwall->Init(go->horizontalPosition + sideOffset, go->verticalPosition + centerOffset, currentDirection);
				}
				

				game_objects->insert(lightwall);
			}

			timer = 0;
			prevDirection = currentDirection;
			prevPosX = go->horizontalPosition;
			prevPosY = go->verticalPosition;
		}
		}
		
	//if (keys.fire)
	//{
	//	if (CanFire())
	//	{
	//		if (dt != 0) {
	//			// fetches a rocket from the pool and use it in game_objects
	//			Rocket* rocket = rockets_pool->FirstAvailable();
	//			if (rocket != NULL)	// rocket is NULL is the object pool can not provide an object
	//			{
	//				rocket->Init(go->horizontalPosition);
	//				game_objects->insert(rocket);
	//			}
	//		}
	//	}
	//}
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

// return true if enough time has passed from the previous rocket
bool PlayerBehaviourComponent::CanFire()
{
	// shoot just if enough time passed by
	if ((engine->getElapsedTime() - time_fire_pressed) < (fire_time_interval))
		return false;

	time_fire_pressed = engine->getElapsedTime();

	SDL_Log("fire!");
	return true;
}

float PlayerBehaviourComponent::getCurrentDirection() {
	return currentDirection;
}

void PlayerRenderComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, std::vector<const char*> sprite_names, PlayerBehaviourComponent* playerBehaviourRef)
{
	Component::Create(engine, go, game_objects);
	this->playerBehaviourRef = playerBehaviourRef;

	//std::cout << "ref address" << this->playerBehaviourRef  << std::endl;

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
	if (playerBehaviourRef->getCurrentDirection() == 0) {
		//std::cout << "showing going up" << std::endl;
		sprites[0]->draw(int(go->horizontalPosition - offset), int(go->verticalPosition));
	}
	if (playerBehaviourRef->getCurrentDirection() == 1) {
		sprites[1]->draw(int(go->horizontalPosition), int(go->verticalPosition - offset));
	}
	if (playerBehaviourRef->getCurrentDirection() == 2) {
		sprites[2]->draw(int(go->horizontalPosition - offset), int(go->verticalPosition));
	}
	if (playerBehaviourRef->getCurrentDirection() == 3) {
		sprites[3]->draw(int(go->horizontalPosition), int(go->verticalPosition - offset));
	}
		
}

void PlayerRenderComponent::Destroy() 
{
	for (int i = 0; i < sprites.size(); i++) {
		if (sprites[i] != NULL)
			sprites[i]->destroy();
		sprites[i] = NULL;
	}
}

void Player::Init()
{
	SDL_Log("Player::Init");
	GameObject::Init();
	lives = 2;
	id = "player";
}

void Player::Receive(Message m)
{
	if (m == HIT)
	{
		SDL_Log("Player::Hit!");
		RemoveLife();

		if (lives < 0) {
			SDL_Log("Player::REKT");
			Send(GAME_OVER);
		}
	}
}

void Player::RemoveLife()
{
	lives--;
	SDL_Log("remaining lives %d", lives);
}
