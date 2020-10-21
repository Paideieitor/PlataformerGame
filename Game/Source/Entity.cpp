#include "Entities.h"
#include "Collisions.h"

Entity::Entity() 
{
	toDelete = false;
}

Entity::~Entity()
{
}

bool Entity::Update(float dt)
{
	return true;
}

void Entity::Collision(Collider* c1, Collider* c2)
{

}

fPoint Entity::GetDrawPosition(iPoint size)
{
	fPoint output = position;

	output.x -= size.x / 2;
	output.y -= size.y / 2;

	return output;
}