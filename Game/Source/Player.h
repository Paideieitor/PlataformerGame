#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Entity.h"

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

	std::vector<Entity*> shurikens;

private:

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

#endif //__PLAYER_H__