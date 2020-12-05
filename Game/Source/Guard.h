#ifndef __GUARD_H__
#define __GUARD_H__

#include "Enemy.h"

class Path;

enum class MoveType
{
	NONE,
	JUMP,
	FALL
};

class Guard : public Enemy
{
public:

	Guard(fPoint position, bool flip, Player* parent);
	~Guard();

	bool Update(float dt);

private:

	void Walk(const float& speed, const float& destination);
	void Fall(const fPoint& destination);

	fPoint GetSpecialDestination(MoveType &type);

	bool chasing;
	bool falling;

	fPoint velocity;

	Animation* walk = nullptr;

	Path* path = nullptr;
};

#endif //__GUARD_H__