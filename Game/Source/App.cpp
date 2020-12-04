#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "FadeToBlack.h"
#include "LogoScene.h"
#include "MainMenu.h"
#include "DungeonScene.h"
#include "WinScene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Collisions.h"
#include "Pathfinding.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	fps = 0;
	frameCap = 60;

	dt = 0.0f;

	capped = true;
	vsync = false;

	global.Start();

	configPath = "config.xml";
	savePath = "savegame.xml";

	startLevel1 = false;
	startLevel2 = false;
	toSave = false;
	toLoad = false;
	drawColliders = false;
	godMode = false;
	pathDebug = false;


	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	fade = new FadeToBlack();
	logo = new LogoScene();
	mainmenu = new MainMenu();
	dungeonscene = new DungeonScene();
	winscene = new WinScene();
	entitymanager = new EntityManager();
	map = new Map();
	collisions = new Collisions();

	paths = Pathfinding::GetInstance();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(map);
	AddModule(fade, false);
	AddModule(logo, true);
	AddModule(mainmenu, false);
	AddModule(dungeonscene, false);
	AddModule(winscene, false);
	AddModule(collisions);
	AddModule(entitymanager);
	// render last to swap buffer
	AddModule(render);

	fade->current = logo;
}

// Destructor
App::~App()
{
	// release modules
	while(modules.size() != 0)
	{
		delete* modules.begin();
		modules.erase(modules.begin());
	}

	configFile.reset();
}

// Add a new module to handle
void App::AddModule(Module* module, bool active)
{
	if(active)
		module->Init();
	modules.emplace_back(module);
}

// Called before render is available
bool App::Awake()
{
	if(!LoadConfig())
		return false;

	Module* pModule = NULL;

	title = configApp.child("title").child_value();
	win->SetTitle(title.c_str());

	for(vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(!pModule->Awake(config.child(pModule->name.c_str())))
		{
			LOG("Awaking Error in %s", pModule->name.c_str());
			return false;
		}
	}

	return true;
}

// Called before the first frame
bool App::Start()
{
	Module* pModule = NULL;

	for(vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(pModule->active == false)
			continue;

		if(!pModule->Start())
		{
			LOG("Starting Error in %s", pModule->name.c_str());
			return false;
		}
	}

	return true;
}

// Called each loop iteration
bool App::Update()
{
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		return false;

	if(!PreUpdate())
		return false;

	if(!DoUpdate())
		return false;

	if(!PostUpdate())
		return false;

	FinishUpdate();
	return true;
}

// Load config file
bool App::LoadConfig()
{
	pugi::xml_parse_result result = configFile.load_file(configPath.c_str());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", configPath.c_str(), result.description());
		return false;
	}
	else
	{
		config = configFile.child("config");
		configApp = config.child("app");
	}

	return true;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
	dt = timer.ReadSec();
	frames++;
	timer.Start();

	if(frames == 1)
		dt = 0;
}

// ---------------------------------------------
void App::FinishUpdate()
{
	if(input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
		startLevel1 = true;
	if(input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
		startLevel2 = true;
	if(input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		toSave = true;
	if(input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		toLoad = true;
	if (input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		pathDebug = !pathDebug;
	if(input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		drawColliders = !drawColliders;
	if(input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
		godMode = !godMode;
	if(input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		capped = !capped;

	if(toSave)
	{
		SaveGame();
		toSave = false;
	}

	if(toLoad)
	{
		LoadGame();
		toLoad = false;
	}

	int miliseconds = timer.Read();
	fps = 1 / dt;

	int c = 0;
	if(capped)
		c = frameCap;

	int avFps = frames / global.ReadSec();

	char title[256];
	sprintf_s(title, 256, "PlatformGame || FPS: %d / Average FPS: %d Last Frame Ms: %d / Cap: %d / dt: %f", fps, avFps, miliseconds, c, dt);
	app->win->SetTitle(title);

	if(capped)
		if((miliseconds < (1000 / frameCap)))
			SDL_Delay((1000 / frameCap) - miliseconds);

}

// Call modules to save data in the save file
bool App::SaveGame()
{
	Module* pModule = NULL;

	pugi::xml_document saveDoc;
	pugi::xml_parse_result result = saveDoc.load_file(savePath.c_str());

	pugi::xml_node mainNode = saveDoc.child("save");
	if(mainNode == NULL)
		mainNode = saveDoc.append_child("save");

	pugi::xml_node moduleNode;
	for(vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		moduleNode = mainNode.child(pModule->name.c_str());
		if(moduleNode == NULL)
			moduleNode = saveDoc.child("save").append_child(pModule->name.c_str());

		if(!pModule->Save(moduleNode))
		{
			LOG("Saving Error in %s. Saving aborted", pModule->name.c_str());
			return false;
		}
	}
	saveDoc.save_file(savePath.c_str());

	return true;
}

// Call modules to load data from the save file
bool App::LoadGame()
{
	Module* pModule = NULL;

	pugi::xml_document saveDoc;
	pugi::xml_parse_result result = saveDoc.load_file(savePath.c_str());

	for(vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		pugi::xml_node mNode = saveDoc.child("save").child(pModule->name.c_str());
		if(mNode == NULL)
			return false;

		if(!pModule->Load(mNode))
		{
			LOG("Loading Error in %s", pModule->name.c_str());
			return false;
		}
	}

	return true;
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	Module* pModule = NULL;

	for(vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(pModule->active == false)
			continue;

		if(!pModule->PreUpdate())
		{
			LOG("PreUpdate Error in %s", pModule->name.c_str());
			return false;
		}
	}

	return true;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	Module* pModule = NULL;

	for(vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(pModule->active == false)
			continue;

		if(!pModule->Update(dt))
		{
			LOG("Update Error in %s", pModule->name.c_str());
			return false;
		}
	}

	return true;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	Module* pModule = NULL;

	for (vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(pModule->active == false)
			continue;

		if(!pModule->PostUpdate())
		{
			LOG("PostUpdate Error in %s", pModule->name.c_str());
			return false;
		}
	}

	return true;
}

// Called before quitting
bool App::CleanUp()
{
	Module* pModule = NULL;

	for(vector<Module*>::iterator m = modules.end() - 1; m != modules.begin(); m--)
	{
		pModule = *m;

		if(!pModule->CleanUp())
		{
			LOG("Clean Up Error in %s", pModule->name.c_str());
			return false;
		}
	}

	delete paths;

	return true;
}

// ---------------------------------------
int App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* App::GetTitle() const
{
	return title.c_str();
}

// ---------------------------------------
const char* App::GetOrganization() const
{
	return organization.c_str();
}

pugi::xml_document* App::GetConfig(pugi::xml_node& node)
{
	node = config;
	return &configFile;
}