#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "LoseScene.h"

#include "Defs.h"
#include "Log.h"

LoseScene::LoseScene() : Module()
{
	name = "mainmenu";
}

LoseScene::~LoseScene()
{}

bool LoseScene::Awake(pugi::xml_node& node)
{
	LOG("Loading Lose Scene");

	return true;
}

bool LoseScene::Start()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	lose = app->tex->Load("Assets/textures/bern_lose.png");

	app->render->SetBackgroundColor({ 255,255,255,255 });

	return true;
}

bool LoseScene::PreUpdate()
{
	return true;
}

bool LoseScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->ChangeScene(this, app->mainmenu);
	}

	app->render->SetTextureEvent(2, lose, { 0,0 });

	return true;
}

bool LoseScene::PostUpdate()
{
	return true;
}

bool LoseScene::CleanUp()
{
	app->tex->UnLoad(lose);

	return true;
}