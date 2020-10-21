#ifndef __DUNGEONSCENE_H__
#define __DUNGEONSCENE_H__

#include "Module.h"

struct Collider;
class WayPoints;

class DungeonScene : public Module
{
public:

	DungeonScene();
	virtual ~DungeonScene();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

private:

	WayPoints* respawn;
};

#endif //__DUNGEONSCENE_H__