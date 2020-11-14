#ifndef __COLLIDER_H__
#define __COLLIDER_H__

#include "Entities.h"

#include "SDL.h"

enum class ColliderType
{
	PLAYER,
	SHURIKEN,
	STATIC_SHURIKEN,
	ATTACK,
	CHECKPOINT,
	GROUND,
	ENEMY
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
	Entity* callback = nullptr;

	bool toDelete;
};

#endif //__COLLIDER_H__