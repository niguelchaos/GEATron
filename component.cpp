#include "component.h"
#include "game_object.h"
#include "avancezlib.h"

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
	for (auto i = 0; i < coll_objects->pool.size(); i++)
	{
		GameObject * go0 = coll_objects->pool[i];
		if (go0->enabled)
		{
			if ((go0->horizontalPosition > go->horizontalPosition - 10) &&
				(go0->horizontalPosition < go->horizontalPosition + 10) &&
				(go0->verticalPosition   > go->verticalPosition - 10) &&
				(go0->verticalPosition   < go->verticalPosition + 10))
			{
				go->Receive(HIT);
				go0->Receive(HIT);
			}
		}
	}
}

void BoxCollideComponent::Create(AvancezLib* engine, GameObject* go, std::set<GameObject*>* game_objects, ObjectPool<GameObject>* coll_objects, int x, int y, UniGrid* uniGrid) {
	Component::Create(engine, go, game_objects);
	this->coll_objects = coll_objects;
	this->x = x;
	this->y = y;
}

void BoxCollideComponent::Update(double dt)
{
	int ballRadius = 16;
	int ballBorder = 3;
	// take game objects x
	//BoxCollideComponent* boxCollider = go->GetComponent<BoxCollideComponent*>();

	//If we are using a uniform grid, instead query the grid. The grid should then return a number of potential ball objects that we then check for collisions with.
	//for (auto i = 0; i < coll_objects->pool.size(); i++)
	//{
	//	GameObject* go0 = coll_objects->pool[i];

	//	if (go0->enabled)
	//	{
	//		CircleCollideComponent* otherCollider = go0->GetComponent<CircleCollideComponent*>();
	//		if (otherCollider != nullptr) { //if the other GameObject doesn't have a CircleColliderComponent we shouldn't go in here...

	//			//Write your solution here...
	//			//Vector2D goTogo0 = go0->position - go->position;
	//			Vector2D goTogo0 = go0->position;
	//			// absolute distance between two game objects = |vec(x,y)| = sqrt( x ^ 2 + y ^2)
	//			float distanceBetweenCircleCenters = sqrt(pow(goTogo0.x, 2) + pow(goTogo0.y, 2));
	//			//float outsideBoxDistance = x + otherCollider->;
	//			bool intersection = false;
	//			//float distanceBetweenCircleCenters = 1;
	//			//bool outsideBoxCheck = go0->position.x + 

	//			if (go0->position.x < -3 || go0->position.x > boxCollider->x - 29 ||
	//				go0->position.y < -3 || go0->position.y > boxCollider->y - 29) {
	//				intersection = true;
	//			}

	//			if (intersection) {
	//				//RigidBodyComponent* rigidBodyComponent = go->GetComponent<RigidBodyComponent*>();
	//				RigidBodyComponent* rigidBodyComponent0 = go0->GetComponent<RigidBodyComponent*>();

	//				// normalized position
	//				Vector2D goTogo0Normalized = goTogo0 / distanceBetweenCircleCenters;
	//				//double dotProduct = rigidBodyComponent->velocity.dotProduct(goTogo0Normalized);
	//				//double dotProduct0 = rigidBodyComponent0->velocity.dotProduct(goTogo0Normalized);
	//				//double dotProduct0 = rigidBodyComponent0->velocity

	//				//rigidBodyComponent->velocity = goTogo0Normalized * dotProduct0;
	//				if (go0->position.x < (0 - ballBorder)) {
	//					rigidBodyComponent0->velocity = rigidBodyComponent0->velocity * Vector2D(-1, 1);
	//					go0->position.x = (0 - ballBorder);
	//				}
	//				if (go0->position.x > boxCollider->x - (32 - ballBorder)) {
	//					rigidBodyComponent0->velocity = rigidBodyComponent0->velocity * Vector2D(-1, 1);
	//					go0->position.x = boxCollider->x - (32 - ballBorder);
	//				}
	//				if (go0->position.y < (0 - ballBorder)) {
	//					rigidBodyComponent0->velocity = rigidBodyComponent0->velocity * Vector2D(1, -1);
	//					go0->position.y = (0 - ballBorder);
	//				}
	//				if (go0->position.y > boxCollider->y - (32 - ballBorder)) {
	//					rigidBodyComponent0->velocity = rigidBodyComponent0->velocity * Vector2D(1, -1);
	//					go0->position.y = boxCollider->y - (32 - ballBorder);
	//				}
	//			}
	//		}
	//	}
	//}
}