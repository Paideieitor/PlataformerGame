#ifndef __APP_H__
#define __APP_H__

#include "Module.h"
#include "Timer.h"

#include "pugixml.hpp"

#include <vector>
#include <string>

// Modules
class Window;
class Input;
class Render;
class Textures;
class Audio;
class FadeToBlack;
class LogoScene;
class MainMenu;
class DungeonScene;
class WinScene;
class EntityManager;
class Map;
class Collisions;

class App
{
public:

	// Constructor
	App(int argc, char* args[]);

	// Destructor
	virtual ~App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(Module* module, bool active = true);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	pugi::xml_document* GetConfig(pugi::xml_node&);

private:

	// Load config file
	bool LoadConfig();

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Call modules to save data in the save file
	bool SaveGame();

	// Call modules to load data from the save file
	bool LoadGame();

public:

	// Modules
	Window* win;
	Input* input;
	Render* render;
	Textures* tex;
	Audio* audio;
	FadeToBlack* fade;
	LogoScene* logo;
	MainMenu* mainmenu;
	DungeonScene* dungeonscene;
	WinScene* winscene;
	EntityManager* entitymanager;
	Map* map;
	Collisions* collisions;

	// Save & Load
	string configPath;
	string savePath;
	bool toSave;
	bool toLoad;

	//Debug
	bool startLevel1;
	bool startLevel2;
	bool drawColliders;
	bool godMode;

private:

	int argc;
	char** args;
	string title;
	string organization;

	vector<Module*> modules;

	pugi::xml_document configFile;
	pugi::xml_node config;
	pugi::xml_node configApp;

	uint frames;
	float dt;
	int fps;
	Timer timer;
	bool capped;
	int frameCap;
};

extern App* app;

#endif	// __APP_H__