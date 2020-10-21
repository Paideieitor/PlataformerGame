#include "DungeonScene.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "EntityManager.h"
#include "Map.h"
#include "Collisions.h"
#include "Checkpoints.h"

#include "Defs.h"
#include "Log.h"

DungeonScene::DungeonScene() : Module()
{
	name = "dungeon";
}

DungeonScene::~DungeonScene()
{

}

bool DungeonScene::Awake(pugi::xml_node& node)
{
	LOG("Loading Dungeon scene");

	return true;
}

bool DungeonScene::Start()
{
	app->entitymanager->Init();
	app->collisions->Init();
	
	respawn = app->map->LoadMap("Assets/maps/dungeon.tmx");

	app->entitymanager->CreateEntity(EntityType::PLAYER, { 100,100 });

	app->render->SetBackgroundColor({ 31,31,31,255 });

	return true;
}

bool DungeonScene::PreUpdate()
{
	return true;
}

bool DungeonScene::Update(float dt)
{
	app->map->DrawMap();

	return true;
}

bool DungeonScene::PostUpdate()
{
	if(app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		app->fade->ChangeScene(this, app->mainmenu);

	return true;
}

bool DungeonScene::CleanUp()
{
	app->entitymanager->Exit();
	app->collisions->Exit();
	app->map->CleanUp();

	return true;
}