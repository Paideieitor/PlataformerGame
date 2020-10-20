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

	app->map->LoadMap("Assets/maps/dungeon.tmx");
	app->entitymanager->CreateEntity(EntityType::PLAYER, { 0,0 });
	app->collisions->CreateCollider(ColliderType::GROUND, { 0,100,500,30 });
	app->collisions->CreateCollider(ColliderType::GROUND, { 30,70,30,30 });
	app->collisions->CreateCollider(ColliderType::GROUND, { 90,50,30,30 });

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