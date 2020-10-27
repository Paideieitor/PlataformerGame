#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "LogoScene.h"

#include "Defs.h"
#include "Log.h"

LogoScene::LogoScene() : Module()
{
	name = "logo";
}

LogoScene::~LogoScene()
{}

bool LogoScene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool LogoScene::Start()
{
	logo = app->tex->Load("Assets/textures/Logo.png");
	aTimer = 0;
	timer = 0;
	stop = false;

	app->render->camera.x = 0;
	app->render->camera.y = 0;

	app->render->SetBackgroundColor({ 255,255,255,255 });

	return true;
}

bool LogoScene::PreUpdate()
{
	return true;
}

bool LogoScene::Update(float dt)
{
	if(!stop)
	{
		if(aTimer == 0)
		{
			timer += dt;
			if(timer >= 1)
			{
				aTimer += dt;
				timer = 0;
			}
		}
		else
		{
			aTimer += dt;
			if(aTimer >= 1)
			{
				aTimer = 1;
				timer += dt;
				if(timer >= 1)
				{
					stop = true;
					app->fade->ChangeScene(this, app->mainmenu);
				}
			}
		}
	}
	if(app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->ChangeScene(this, app->mainmenu);

	app->render->SetTextureEvent(1, logo, { 0, 0 }, { 0,0,0,0 }, false, true, 1.0f, aTimer * 255);
	
	return true;
}

bool LogoScene::PostUpdate()
{
	return true;
}

bool LogoScene::CleanUp()
{
	LOG("Freeing scene");

	aTimer = 0;
	timer = 0;
	stop = false;

	app->tex->UnLoad(logo);

	return true;
}
