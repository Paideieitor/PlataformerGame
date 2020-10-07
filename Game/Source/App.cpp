#include "App.h"
#include "Window.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "Audio.h"
#include "Scene.h"

#include "Defs.h"
#include "Log.h"

#include <iostream>
#include <sstream>

// Constructor
App::App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;

	saveFile = "savegame.xml";
	toSave = true;
	toLoad = false;

	input = new Input();
	win = new Window();
	render = new Render();
	tex = new Textures();
	audio = new Audio();
	scene = new Scene();

	// Ordered for awake / Start / Update
	// Reverse order of CleanUp
	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	AddModule(scene);

	// render last to swap buffer
	AddModule(render);
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

void App::AddModule(Module* module)
{
	module->Init();
	modules.emplace_back(module);
}

// Called before render is available
bool App::Awake()
{
	bool ret = LoadConfig();
	Module* pModule = NULL;

	title = configApp.child("title").child_value();
	win->SetTitle(title.c_str());

	if(ret == true)
	{
		for(std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
		{
			pModule = *m;

			if(!pModule->Awake(config.child(pModule->name.c_str())))
				ret = false;
		}
	}

	return ret;
}

// Called before the first frame
bool App::Start()
{
	bool ret = true;
	Module* pModule = NULL;

	for(std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(!pModule->Start())
			ret = false;
	}

	return ret;
}

// Called each loop iteration
bool App::Update()
{
	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

bool App::LoadConfig()
{
	bool ret = true;

	pugi::xml_parse_result result = configFile.load_file("config.xml");

	if(result == NULL)
	{
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
		ret = false;
	}
	else
	{
		config = configFile.child("config");
		configApp = config.child("app");
	}

	return ret;
}

// ---------------------------------------------
void App::PrepareUpdate()
{
}

// ---------------------------------------------
void App::FinishUpdate()
{
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
}

// Call modules to save data in the save file
bool App::SaveGame()
{
	bool ret = true;
	Module* pModule = NULL;

	pugi::xml_document saveDoc;
	pugi::xml_parse_result result = saveDoc.load_file(saveFile.c_str());

	for(std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		pugi::xml_node mNode = saveDoc.child("save").child(pModule->name.c_str());
		if(mNode == NULL)
			mNode = saveDoc.child("save").append_child(pModule->name.c_str());

		if (!pModule->Save(mNode))
		{
			ret = false;
			break;
		}
		saveDoc.save_file(saveFile.c_str());
	}

	return ret;
}

// Call modules to load data from the save file
bool App::LoadGame()
{
	bool ret = true;
	Module* pModule = NULL;

	for(std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(!pModule->Load(config.child(pModule->name.c_str())))
			ret = false;
	}

	return ret;
}

// Call modules before each loop iteration
bool App::PreUpdate()
{
	bool ret = true;
	Module* pModule = NULL;

	for(std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(pModule->active == false)
			continue;

		if(!pModule->PreUpdate())
			ret = false;
	}

	return ret;
}

// Call modules on each loop iteration
bool App::DoUpdate()
{
	bool ret = true;
	Module* pModule = NULL;

	for(std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(pModule->active == false)
			continue;

		if(!pModule->Update(dt))
			ret = false;
	}

	return ret;
}

// Call modules after each loop iteration
bool App::PostUpdate()
{
	bool ret = true;
	Module* pModule = NULL;

	for (std::vector<Module*>::iterator m = modules.begin(); m != modules.end(); m++)
	{
		pModule = *m;

		if(pModule->active == false)
			continue;

		if(!pModule->PostUpdate())
			ret = false;
	}

	return ret;
}

// Called before quitting
bool App::CleanUp()
{
	bool ret = true;
	Module* pModule = NULL;

	for(std::vector<Module*>::iterator m = modules.end() - 1; m != modules.begin(); m--)
	{
		pModule = *m;

		if (!pModule->CleanUp())
			return false;
	}

	return ret;
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