#ifndef __DUNGEONSCENE_H__
#define __DUNGEONSCENE_H__

#include "Module.h"
#include "Point.h"

struct Collider;
class WayPoints;
class Entity;

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

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	bool CleanUp();

	void IterateCheckpoint();
	void RespawnPlayer();

	bool iterate;

	uint jumpSound;
	uint deathSound;

private:

	WayPoints* respawn = nullptr;
	Entity* player = nullptr;
	Collider* checkpoint = nullptr;

	void UpdateCheckpoint();
	void ResetCheckpoint();

	void LoadCurrentMap();

	int levels;
	int currentLevel;
	int currentCheckpoint;

	bool keepMusic;
};

#endif //__DUNGEONSCENE_H__