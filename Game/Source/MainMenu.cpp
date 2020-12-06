#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "DungeonScene.h"
#include "MainMenu.h"

#include "Defs.h"
#include "Log.h"

MainMenu::MainMenu() : Module()
{
	name = "mainmenu";
}

MainMenu::~MainMenu()
{

}

bool MainMenu::Awake(pugi::xml_node& node)
{
	LOG("Loading Main menu");

	return true;
}

bool MainMenu::Start()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	bern = app->tex->Load("Assets/textures/bern.png");
	app->audio->PlayMusic("Assets/audio/music/menu_song.ogg", 0.0f);

	app->render->SetBackgroundColor({ 255,255,255,255 });

	return true;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		app->input->quit = true;

	if(app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->toLoad = true;

	app->render->SetTextureEvent(2, bern, { 0,0 });

	return true;
}

bool MainMenu::PostUpdate()
{
	return true;
}

bool MainMenu::CleanUp()
{
	app->tex->UnLoad(bern);

	return true;
}