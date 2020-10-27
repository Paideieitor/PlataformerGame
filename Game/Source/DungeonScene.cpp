#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "WinScene.h"
#include "EntityManager.h"
#include "Map.h"
#include "Collisions.h"
#include "Checkpoints.h"
#include "DungeonScene.h"

#include "Defs.h"
#include "Log.h"

#define MAP_PATH "Assets/maps/dungeon"
#define MAP_FILETYPE ".tmx"
#define CHECKPOINT_SIZE 16
#define CAMERA_SPEED 150

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

	iterate = false;

	respawn = nullptr;
	player = nullptr;
	checkpoint = nullptr;

	levels = node.child("levels").attribute("amount").as_int();
	currentLevel = 1;
	currentCheckpoint = 0;

	return true;
}

bool DungeonScene::Start()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	LoadCurrentMap();

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
	if(iterate || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
	{
		iterate = false;
		IterateCheckpoint();
	}
	if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
		ResetCheckpoint();

	if(app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
		RespawnPlayer();

	app->map->DrawMap();

	return true;
}

bool DungeonScene::PostUpdate()
{
	if(app->startLevel1)
	{
		app->startLevel1 = false;
		currentLevel = 1;
		app->fade->ChangeScene(this, this);
	}
	if(app->startLevel2)
	{
		app->startLevel2 = false;
		currentLevel = 2;
		app->fade->ChangeScene(this, this);
	}
	if(app->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		app->toSave = true;
		app->fade->ChangeScene(this, app->mainmenu);
	}

	return true;
}

bool DungeonScene::Save(pugi::xml_node& node)
{
	pugi::xml_node dNode = node.child("current");
	if(!dNode)
		dNode = node.append_child("current");
	pugi::xml_attribute atr = dNode.attribute("level");
	if(!atr)
		atr = dNode.append_attribute("level");
	atr.set_value(currentLevel);
	atr = dNode.attribute("checkpoint");
	if (!atr)
		atr = dNode.append_attribute("checkpoint");
	atr.set_value(respawn->GetCurrent());

	return true;
}

bool DungeonScene::Load(pugi::xml_node& node)
{
	currentLevel = node.child("current").attribute("level").as_int();
	currentCheckpoint = node.child("current").attribute("checkpoint").as_int();

	app->fade->ChangeScene(app->fade->current, this);

	return true;
}

bool DungeonScene::CleanUp()
{
	delete respawn;

	app->entitymanager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();

	respawn = nullptr;
	player = nullptr;
	checkpoint = nullptr;

	return true;
}

void DungeonScene::IterateCheckpoint()
{
	if(respawn->Iterate())
	{
		UpdateCheckpoint();
	}
	else
	{
		currentLevel++;
		if(currentLevel > levels)
		{
			currentCheckpoint = 0;
			respawn->Reset();
			app->fade->ChangeScene(this, app->winscene);
			currentLevel = 1;
		}
		else
		{
			this->CleanUp();
			this->Start();
		}
	}
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
	if(checkpoint)
		app->collisions->DeleteCollider(checkpoint);
	iPoint cPosition = respawn->checkpoints[respawn->GetCurrent() + 1].position;
	int cX = cPosition.x - CHECKPOINT_SIZE / 2;
	int cY = cPosition.y - CHECKPOINT_SIZE / 2;
	checkpoint = app->collisions->CreateCollider(ColliderType::CHECKPOINT, { cX,cY,CHECKPOINT_SIZE ,CHECKPOINT_SIZE });
}

void DungeonScene::ResetCheckpoint()
{
	respawn->Reset();
	UpdateCheckpoint();
}

void DungeonScene::LoadCurrentMap()
{
	char path[64];
	sprintf_s(path, "%s%d%s", MAP_PATH, currentLevel, MAP_FILETYPE);

	app->map->CleanUp();
	if(respawn)
		delete respawn;
	respawn = app->map->LoadMap(path);
	respawn->SetCurrent(currentCheckpoint);
	currentCheckpoint = 0;
}