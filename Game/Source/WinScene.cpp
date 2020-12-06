#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "WinScene.h"

#include "Defs.h"
#include "Log.h"

WinScene::WinScene() : Module()
{
	name = "mainmenu";
}

WinScene::~WinScene()
{}

bool WinScene::Awake(pugi::xml_node& node)
{
	LOG("Loading Main menu");

	return true;
}

bool WinScene::Start()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	win = app->tex->Load("Assets/textures/bern_win.png");
	app->audio->PlayMusic("Assets/audio/music/win_song.ogg", 0.0f);

	app->render->SetBackgroundColor({ 255,255,255,255 });

	return true;
}

bool WinScene::PreUpdate()
{
	return true;
}

bool WinScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		app->input->quit = true;

	if(app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
	{
		app->fade->ChangeScene(this, app->mainmenu);
	}

	app->render->SetTextureEvent(2, win, { 0,0 });

	return true;
}

bool WinScene::PostUpdate()
{
	return true;
}

bool WinScene::CleanUp()
{
	app->tex->UnLoad(win);

	return true;
}