#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Point.h"
#include "Log.h"

struct Collider;
class Animation;

enum class EntityType
{
	ENTITY,
	PLAYER,
	SHURIKEN,
	ENEMY,
	BAT,
	GUARD
};

class Player;

class Entity
{

public:

	Entity(EntityType type, fPoint position, bool flip, Player* parent);
	virtual ~Entity();

	virtual bool Update(float dt);

	virtual void Collision(Collider* c1, Collider* c2);

	bool toDelete;
	bool toRemove;

	EntityType type;

	fPoint position;
	iPoint size;

	Collider* body = nullptr;

	Player* parent = nullptr;

protected:

	bool flip;

	fPoint GetDrawPosition(iPoint size);
	SDL_Texture* texture = nullptr;

	Animation* currentAnimation = nullptr;
};

#endif //__ENTITY_H__