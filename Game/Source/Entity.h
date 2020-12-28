#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Point.h"
#include "Log.h"

struct Collider;
class Animation;
class Player;
class Element;

struct ElementData;

enum class EntityType
{
	ENTITY,
	PLAYER,
	SHURIKEN,
	ENEMY,
	BAT,
	GUARD
};

class Entity
{

public:

	Entity(EntityType type, fPoint position, bool flip, Player* parent);
	virtual ~Entity();

	virtual bool Update(float dt);

	virtual void Collision(Collider* c1, Collider* c2);

	virtual void UIEvent(Element*, ElementData&);

	bool toDelete;
	bool toRemove;
	bool flip;

	EntityType type;

	fPoint position;
	iPoint size;

	Collider* body = nullptr;

	Player* parent = nullptr;

protected:

	fPoint GetDrawPosition(iPoint size);
	SDL_Texture* texture = nullptr;

	Animation* currentAnimation = nullptr;
};

#endif //__ENTITY_H__