#include "LogoScene.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "MainMenu.h"
#include "FadeToBlack.h"

#include "Defs.h"
#include "Log.h"

LogoScene::LogoScene() : Module()
{
	name = "scene";
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

		if(!stop && app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			stop = true;
			app->fade->ChangeScene(this, app->mainmenu);
		}
	}

	app->render->SetTextureEvent(1, logo, { 380, 100 }, { 0,0,0,0 }, true, 1.0f, aTimer * 255);
	
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
