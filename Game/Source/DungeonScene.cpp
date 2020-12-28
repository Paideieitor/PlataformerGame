#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "WinScene.h"
#include "LoseScene.h"
#include "EntityManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Map.h"
#include "Collisions.h"
#include "Checkpoints.h"
#include "Options.h"
#include "Pause.h"
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

	notEntities = false;

	levels = node.child("levels").attribute("amount").as_int();
	currentLevel = 1;
	currentCheckpoint = 0;

	return true;
}

bool DungeonScene::Start()
{
	jumpSound = app->audio->LoadFx("Assets/Audio/Fx/jump.wav");
	deathSound = app->audio->LoadFx("Assets/Audio/Fx/oof.wav");
	batSound = app->audio->LoadFx("Assets/Audio/Fx/bat.wav");
	guardAlertSound = app->audio->LoadFx("Assets/Audio/Fx/guard_alert.wav");
	guardStopChaseSound = app->audio->LoadFx("Assets/Audio/Fx/huh.wav");

	if(!keepMusic)
		app->audio->PlayMusic("Assets/Audio/Music/dungeon_song.ogg", 0.0f);
	keepMusic = false;
	notEntities = false;

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
	if(dt != 0)
	{
		if(iterate || app->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN)
		{
			iterate = false;
			IterateCheckpoint();
		}

		if(app->startCurrentLevel)
		{
			app->startCurrentLevel = false;
			ResetCheckpoint();
			RespawnPlayer();
		}

		if(app->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN)
			ResetCheckpoint();

		if(app->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN)
			RespawnPlayer();
	}
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
	if(!app->options->active && app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if(!app->pause->active)
			app->pause->Activate();
		else
			app->pause->Deactivate();
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

		if (entity->type == EntityType::BAT || entity->type == EntityType::GUARD || entity->type == EntityType::SHURIKEN)
			enemies.push_back(entity);
	}
	
	node.remove_child("enemies");
	dNode.remove_attribute("x");
	dNode.remove_attribute("y");
	if (enemies.size() == 0 || notEntities)
		return true;

	if (player)
	{
		atr = dNode.append_attribute("x");
		atr.set_value(player->position.x);

		atr = dNode.append_attribute("y");
		atr.set_value(player->position.y);
	}

	dNode = node.append_child("enemies");
	atr = dNode.attribute("amount");
	if(!atr)
		atr = dNode.append_attribute("amount");
	atr.set_value(enemies.size());

	pugi::xml_node eNode;
	for(uint i = 0; i < enemies.size(); i++)
	{
		eNode = dNode.append_child("enemy");

		atr = eNode.append_attribute("type");
		if(enemies[i]->type == EntityType::BAT)
		{
			Enemy* bat = (Enemy*)enemies[i];
			atr.set_value("BAT");

			atr = eNode.append_attribute("x");
			atr.set_value(enemies[i]->position.x);

			atr = eNode.append_attribute("y");
			atr.set_value(enemies[i]->position.y);

			atr = eNode.append_attribute("resting");
			atr.set_value(bat->resting);
		}
		else if(enemies[i]->type == EntityType::GUARD)
		{
			Enemy* guard = (Enemy*)enemies[i];
			atr.set_value("GUARD");

			atr = eNode.append_attribute("x");
			atr.set_value(guard->dest.x);

			atr = eNode.append_attribute("y");
			atr.set_value(guard->dest.y);
		}
		else if(enemies[i]->type == EntityType::SHURIKEN)
		{
			atr.set_value("SHURIKEN");

			atr = eNode.append_attribute("x");
			atr.set_value(enemies[i]->position.x);

			atr = eNode.append_attribute("y");
			atr.set_value(enemies[i]->position.y);
		}

		atr = eNode.append_attribute("flip");
		atr.set_value(enemies[i]->flip);
	}

	return true;
}

bool DungeonScene::Load(pugi::xml_node& node)
{
	bool empty = false;
	if(node.child("current").attribute("level"))
		currentLevel = node.child("current").attribute("level").as_int();
	else
	{
		currentLevel = 1;
		empty = true;
	}
	if(node.child("current").attribute("checkpoint"))
		currentCheckpoint = node.child("current").attribute("checkpoint").as_int();
	else
	{
		currentCheckpoint = 0;
		empty = true;
	}
	if (!empty)
	{
		if (node.child("current").attribute("x") && node.child("current").attribute("y"))
		{
			loadPosition = new fPoint();
			loadPosition->x = node.child("current").attribute("x").as_float();
			loadPosition->y = node.child("current").attribute("y").as_float();
		}

		node = node.child("enemies");
		if (node)
		{
			loadedEnemies = new vector<EnemyInfo>;
			for (pugi::xml_node oNode = node.child("enemy"); oNode != NULL; oNode = oNode.next_sibling("enemy"))
			{
				EnemyInfo enemy;

				enemy.type = oNode.attribute("type").as_string();

				float x = oNode.attribute("x").as_float();
				float y = oNode.attribute("y").as_float();
				enemy.position = { x,y };

				pugi::xml_attribute atr = oNode.attribute("resting");
				if (!atr)
					enemy.resting = false;
				else
					enemy.resting = atr.as_bool();

				enemy.flip = oNode.attribute("flip").as_bool();

				loadedEnemies->push_back(enemy);
			}
		}
	}
	app->fade->ChangeScene(app->fade->current, this);

	return true;
}

bool DungeonScene::CleanUp()
{
	app->audio->DeleteFx();
	jumpSound = NULL;
	deathSound = NULL;
	batSound = NULL;
	guardAlertSound = NULL;
	guardStopChaseSound = NULL;

	delete respawn;

	app->entitymanager->CleanUp();
	app->collisions->CleanUp();
	app->map->CleanUp();

	respawn = nullptr;
	player = nullptr;
	checkpoint = nullptr;

	return true;
}

void DungeonScene::UIEvent(Element* element, ElementData&)
{
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
			notEntities = true;
			app->toSave = true;
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
	
	if(!loadPosition)
	{
		iPoint position = respawn->checkpoints[respawn->GetCurrent()].position;
		player = app->entitymanager->CreateEntity(EntityType::PLAYER, { (float)position.x,(float)position.y });
	}
	else
	{
		player = app->entitymanager->CreateEntity(EntityType::PLAYER, { loadPosition->x,loadPosition->y - 2.0f });
		delete loadPosition;
		loadPosition = nullptr;
	}


	if (!loadedEnemies)
	{
		vector<EntityData> entities = app->map->GetEntityData();
		for (uint i = 0; i < entities.size(); i++)
			app->entitymanager->CreateEntity(entities[i].type, entities[i].position);
	}
	else
	{
		for(uint i = 0; i < loadedEnemies->size(); i++)
		{
			if ((*loadedEnemies)[i].type == "SHURIKEN")
			{
				Player* pl = (Player*)player;
				pl->shurikens.push_back(app->entitymanager->CreateEntity(EntityType::SHURIKEN, (*loadedEnemies)[i].position, (*loadedEnemies)[i].flip, pl));
			}
			else
			{
				EntityType type = EntityType::ENEMY;
				if ((*loadedEnemies)[i].type == "BAT")
					type = EntityType::BAT;
				else if ((*loadedEnemies)[i].type == "GUARD")
					type = EntityType::GUARD;

				Enemy* enemy = (Enemy*)app->entitymanager->CreateEntity(type, (*loadedEnemies)[i].position, (*loadedEnemies)[i].flip);

				enemy->resting = (*loadedEnemies)[i].resting;
			}
		}

		delete loadedEnemies;
		loadedEnemies = nullptr;
	}
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