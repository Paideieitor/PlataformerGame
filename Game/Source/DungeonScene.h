#ifndef __DUNGEONSCENE_H__
#define __DUNGEONSCENE_H__

#include "Module.h"

struct Collider;

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
};

#endif //__DUNGEONSCENE_H__