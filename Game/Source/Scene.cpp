#include "Scene.h"

#include "App.h"
#include "Input.h"
#include "Textures.h"
#include "Audio.h"
#include "Render.h"
#include "Window.h"
#include "MainMenu.h"
#include "FadeToBlack.h"
#include "Map.h"

#include "Defs.h"
#include "Log.h"

Scene::Scene() : Module()
{
	name = "scene";
}

Scene::~Scene()
{}

bool Scene::Awake(pugi::xml_node& node)
{
	LOG("Loading Scene");
	bool ret = true;

	return ret;
}

bool Scene::Start()
{
	img = app->tex->Load("Assets/textures/test.png");
	app->map->LoadMap("Assets/maps/hello2.tmx");

	return true;
}

bool Scene::PreUpdate()
{
	return true;
}

bool Scene::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT)
		app->render->camera.y += 1;

	if(app->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT)
		app->render->camera.y -= 1;

	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT)
		app->render->camera.x += 1;

	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		app->render->camera.x -= 1;

	if (app->input->GetKey(SDL_SCANCODE_Q) == KEY_DOWN)
		app->fade->ChangeScene(this, app->mainmenu);

	app->render->SetTextureEvent(1, img, { 380, 100 });

	return true;
}

bool Scene::PostUpdate()
{
	bool ret = true;

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool Scene::CleanUp()
{
	LOG("Freeing scene");

	app->tex->UnLoad(img);

	return true;
}
