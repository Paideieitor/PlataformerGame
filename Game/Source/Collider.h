#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "SDL.h"
#include "Entities.h"

enum class ColliderType
{
	PLAYER,
	SHURIKEN,
	ATTACK,
	CHECKPOINT,
	GROUND
};

struct Collider
{
	Collider(ColliderType type, SDL_Rect rect, bool checker, Entity* callback)
	{
		this->type = type;
		this->rect = rect;
		this->checker = checker;
		this->callback = callback;
		toDelete = false;
	}

	ColliderType type;
	SDL_Rect rect;
	bool checker;
	Entity* callback;

	bool toDelete;
};

#endif //__COLLIDER_H__