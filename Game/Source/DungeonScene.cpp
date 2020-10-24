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
#include "Entities.h"

#include "Defs.h"
#include "Log.h"

#define CHECKPOINTSIZE 16

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

	RespawnPlayer();

	UpdateCheckpoint();

	app->render->SetBackgroundColor({ 31,31,31,255 });

	return true;
}

bool DungeonScene::PreUpdate()
{
	return true;
}

bool DungeonScene::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		IterateCheckpoint();
	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		ResetCheckpoint();

	if(app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		RespawnPlayer();
	
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
	delete respawn;

	app->entitymanager->Exit();
	app->collisions->Exit();
	app->map->CleanUp();

	respawn = nullptr;
	player = nullptr;
	checkpoint = nullptr;

	return true;
}

void DungeonScene::IterateCheckpoint()
{
	if(respawn->Iterate())
		UpdateCheckpoint();
}

void DungeonScene::RespawnPlayer()
{
	if(player)
		app->entitymanager->DeleteEntity(player);

	iPoint position = respawn->checkpoints[respawn->GetCurrent()].position;
	player = app->entitymanager->CreateEntity(EntityType::PLAYER, { (float)position.x,(float)position.y });
}

void DungeonScene::UpdateCheckpoint()
{
	if (checkpoint)
		app->collisions->DeleteCollider(checkpoint);
	iPoint cPosition = respawn->checkpoints[respawn->GetCurrent() + 1].position;
	int cX = cPosition.x - CHECKPOINTSIZE / 2;
	int cY = cPosition.y - CHECKPOINTSIZE / 2;
	checkpoint = app->collisions->CreateCollider(ColliderType::CHECKPOINT, { cX,cY,CHECKPOINTSIZE ,CHECKPOINTSIZE });
}

void DungeonScene::ResetCheckpoint()
{
	respawn->Reset();
	UpdateCheckpoint();
}
