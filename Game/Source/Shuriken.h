#ifndef __SHURIKEN_H__
#define __SHURIKEN_H__

#include "Entity.h"

class Shuriken : public Entity
{
public:

	Shuriken(fPoint position, bool flip, Player* parent);
	virtual ~Shuriken();

	bool Update(float dt);

	void Collision(Collider* c1, Collider* c2);

private:

	float velocity;
	bool hit;
	bool wait;
};

#endif //__SHURIKEN_H__