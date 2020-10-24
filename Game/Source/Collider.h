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
		pastPosition = { rect.x,rect.y };
		this->checker = checker;
		this->callback = callback;
		toDelete = false;
	}

	void SetPosition(int x, int y)
	{
		pastPosition = { rect.x,rect.y };
		rect.x = x;
		rect.y = y;
	}

	ColliderType type;
	SDL_Rect rect;
	iPoint pastPosition;
	bool checker;
	Entity* callback;

	bool toDelete;
};

#endif //__COLLIDER_H__