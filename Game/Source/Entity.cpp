#include "Entities.h"

Entity::Entity() 
{
	toDelete = false;
}

Entity::~Entity()
{
	if(texture)
		app->tex->UnLoad(texture);
}

bool Entity::Update(float dt)
{
	return true;
}

fPoint Entity::GetDrawPosition(iPoint size)
{
	fPoint output = position;

	output.x -= size.x;
	output.y += size.y;

	return output;
}