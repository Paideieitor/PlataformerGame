#include "Collisions.h"
#include "Entities.h"

Entity::Entity(EntityType type, fPoint position, bool flip)
{
	this->type = type;
	this->position = position;
	this->flip = flip;

	toDelete = false;
	toRemove = false;
	flip = false;
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