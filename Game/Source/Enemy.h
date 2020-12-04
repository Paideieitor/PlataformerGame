#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "Entity.h"

class Enemy : public Entity
{
public:
	Enemy(EntityType type, fPoint position, bool flip, Player* parent);
	virtual ~Enemy();

	bool Update(float dt);

	bool resting;
};

#endif //__ENEMY_H__