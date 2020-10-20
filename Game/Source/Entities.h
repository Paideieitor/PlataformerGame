#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Point.h"

struct Collider;
class Animation;

enum class EntityType
{
	ENTITY,
	PLAYER
};

class Entity
{

public:

	Entity();
	virtual ~Entity();

	virtual bool Update(float dt);

	virtual void Collision(Collider* c1, Collider* c2);

	bool toDelete;

protected:

	EntityType type;

	fPoint position;
	iPoint size;

	fPoint GetDrawPosition(iPoint size);
	SDL_Texture* texture;

	Animation* currentAnimation;
};

enum class WallCollision
{
	NONE,
	RIGHT,
	LEFT
};

class Player : public Entity
{

public:

	Player(fPoint position);
	virtual ~Player();

	bool Update(float dt);

	void Collision(Collider* c1, Collider* c2);

private:

	Animation* idle;
	Animation* run;
	Animation* jump;

	Collider* body;

	bool flip;
	fPoint velocity;
	WallCollision wall;
	bool grounded;
	float timeOnAir;
};

#endif