#include "MainMenu.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "DungeonScene.h"

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
	bern = app->tex->Load("Assets/textures/BERN.png");

	return true;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		app->fade->ChangeScene(this, app->dungeonscene);

	app->render->SetTextureEvent(2, bern, { 100,100 });

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