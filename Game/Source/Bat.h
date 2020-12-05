#ifndef __BAT_H__
#define __BAT_H__

#include "Enemy.h"

class Path;

class Bat : public Enemy
{
public:

	Bat(fPoint position, bool flip, Player* parent);
	~Bat();

	bool Update(float dt);

private:

	Animation* idle = nullptr;
	Animation* fly = nullptr;

	Path* path = nullptr;
};

#endif //__BAT_H__