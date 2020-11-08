#ifndef __ENTITIES_H__
#define __ENTITIES_H__

#include "App.h"
#include "Render.h"
#include "Textures.h"

#include "Point.h"

struct Collider;
class Animation;

enum class EntityType
{
	ENTITY,
	PLAYER,
	SHURIKEN
};

class Entity
{

public:

	Entity(EntityType type, fPoint position, bool flip);
	virtual ~Entity();

	virtual bool Update(float dt);

	virtual void Collision(Collider* c1, Collider* c2);

	bool toDelete;
	bool toRemove;

	EntityType type;

	fPoint position;
	iPoint size;

	Collider* body = nullptr;

protected:

	bool flip;

	fPoint GetDrawPosition(iPoint size);
	SDL_Texture* texture = nullptr;

	Animation* currentAnimation = nullptr;
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

	Player(fPoint position, bool flip);
	virtual ~Player();

	bool Update(float dt);

	void Collision(Collider* c1, Collider* c2);

private:

	std::vector<Entity*> shurikens;
	float shurikenColdown;
	float shurikenTimer;

	Animation* idle = nullptr;
	Animation* run = nullptr;
	Animation* jump = nullptr;

	Collider* feet = nullptr;

	fPoint velocity;
	WallCollision wall;
	bool grounded;
	float timeOnAir;

	uint jumps;
};

class Shuriken : public Entity
{
public:

	Shuriken(fPoint position, bool flip);
	virtual ~Shuriken();

	bool Update(float dt);

	void Collision(Collider* c1, Collider* c2);

private:

	Collider* ground = nullptr;

	float velocity;
	bool hit;
};

#endif