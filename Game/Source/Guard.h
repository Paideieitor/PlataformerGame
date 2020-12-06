#ifndef __GUARD_H__
#define __GUARD_H__

#include "Enemy.h"

class Path;

enum class MoveType
{
	NONE,
	JUMP,
	FALL,
	ERROR
};

class Guard : public Enemy
{
public:

	Guard(fPoint position, bool flip, Player* parent);
	~Guard();

	bool Update(float dt);

private:

	void Walk(const float& speed, const float& destination);

	fPoint GetSpecialDestination(MoveType &type);

	bool chasing;
	bool falling;
	bool jumping;

	float timeOnAir;

	fPoint velocity;

	Animation* walk = nullptr;
	Animation* chase = nullptr;

	Path* path = nullptr;
};

#endif //__GUARD_H__