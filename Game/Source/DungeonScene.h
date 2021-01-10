#ifndef __DUNGEONSCENE_H__
#define __DUNGEONSCENE_H__

#include "Module.h"
#include "Point.h"

#include <vector>

class WayPoints;
class Entity;
class Timer;

struct Collider;
struct SDL_Texture;

struct EnemyInfo
{
	string type;
	fPoint position;
	bool resting = false;
	bool flip = false;
};

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

	void UIEvent(Element*, ElementData&);

	void IterateCheckpoint();
	void RespawnPlayer();

	bool iterate;

	uint jumpSound;
	uint deathSound;
	uint batSound;
	uint guardAlertSound;
	uint guardStopChaseSound;

	Entity* player = nullptr;

	Timer* gameTimer = nullptr;

private:

	WayPoints* respawn = nullptr;
	Collider* checkpoint = nullptr;

	void UpdateCheckpoint();
	void ResetCheckpoint();

	void LoadCurrentMap();

	int levels;
	int currentLevel;
	int currentCheckpoint;

	bool keepMusic;

	vector<EnemyInfo>* loadedEnemies = nullptr;
	fPoint* loadPosition = nullptr;
	uint* savedTimers;
	bool notEntities;

	SDL_Texture* timerText = nullptr;
	iPoint timerTextSize;
};

#endif //__DUNGEONSCENE_H__