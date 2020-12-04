#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "WinScene.h"
#include "LoseScene.h"
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

	keepMusic = false;

	levels = node.child("levels").attribute("amount").as_int();
	currentLevel = 1;
	currentCheckpoint = 0;

	return true;
}

bool DungeonScene::Start()
{
	jumpSound = app->audio->LoadFx("Assets/audio/fx/jump.wav");
	deathSound = app->audio->LoadFx("Assets/audio/fx/oof.wav");

	if(!keepMusic)
		app->audio->PlayMusic("Assets/audio/music/dungeon_song.ogg", 0.0f);
	keepMusic = false;

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
	if(!atr)
		atr = dNode.append_attribute("checkpoint");
	atr.set_value(respawn->GetCurrent());

	vector<Entity*> enemies;
	for(uint i = 0; i < app->entitymanager->entities.size(); i++)
	{
		Entity* entity = app->entitymanager->entities[i];

		if (entity->type == EntityType::BAT)
			enemies.push_back(entity);
	}
	
	dNode = node.child("enemies");
	if(!dNode)
		dNode = node.append_child("enemies");
	atr = dNode.attribute("amount");
	if(!atr)
		atr = dNode.append_attribute("amount");
	atr.set_value(enemies.size());

	pugi::xml_node eNode;
	for(uint i = 0; i < enemies.size(); i++)
	{
		eNode = dNode.child("enemy");
		if(!eNode)
			eNode = dNode.append_child("enemy");
		atr = eNode.attribute("x");
		if(!atr)
			atr = eNode.append_attribute("x");
		atr.set_value(enemies[i]->position.x);
		atr = eNode.attribute("y");
		if(!atr)
			atr = eNode.append_attribute("y");
		atr.set_value(enemies[i]->position.y);

		//atr = eNode.attribute("resting");
		//if (!atr)
		//	atr = eNode.append_attribute("resting");
		//atr.set_value(enemies[i]->resting);
	}

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
	app->audio->DeleteFx();
	jumpSound = NULL;
	deathSound = NULL;

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
			keepMusic = true;
			this->Start();
		}
	}
}

void DungeonScene::RespawnPlayer()
{
	if(player)
	{
		for(uint i = 0; i < app->entitymanager->entities.size(); i++)
			app->entitymanager->entities[i]->toDelete = true;
		player = nullptr;
	}

	iPoint position = respawn->checkpoints[respawn->GetCurrent()].position;
	player = app->entitymanager->CreateEntity(EntityType::PLAYER, { (float)position.x,(float)position.y });

	vector<EntityData> entities = app->map->GetEntityData();
	for(uint i = 0; i < entities.size(); i++)
		app->entitymanager->CreateEntity(entities[i].type, entities[i].position);
}

void DungeonScene::UpdateCheckpoint()
{
	if(checkpoint)
	{
		app->collisions->DeleteCollider(checkpoint);
		checkpoint = nullptr;
	}
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