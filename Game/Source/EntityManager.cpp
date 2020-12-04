#include "EntityManager.h"
#include "Player.h"
#include "Shuriken.h"
#include "Bat.h"
#include "Guard.h"

EntityManager::EntityManager()
{
	name = "entitymanager";
}

EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& node)
{
	return true;
}

bool EntityManager::Start()
{
	return true;
}

bool EntityManager::PreUpdate()
{
	return true;
}

bool EntityManager::Update(float dt)
{
	for(vector<Entity*>::iterator e = entities.begin(); e != entities.end(); e++)
	{
		Entity* entity = *e;

		if(!entity->toDelete)
		{
			if(!entity->Update(dt))
			{
				return false;
			}
		}
		else
			FastDeleteEntity(e);
	}

	int updater = 0;
	for(vector<vector<Entity*>::iterator>::iterator i = toDelete.begin(); i != toDelete.end(); i++)
	{
		vector<Entity*>::iterator eItr = *i - updater;
		delete *eItr;
		entities.erase(eItr);
		updater++;
	}
	toDelete.erase(toDelete.begin(), toDelete.end());	

	return true;
}
bool EntityManager::PostUpdate()
{
	for(vector<Entity*>::iterator e = buffer.begin(); e != buffer.end(); e++)
		entities.push_back(*e);
	buffer.erase(buffer.begin(), buffer.end());

	return true;
}

bool EntityManager::Save(pugi::xml_node& node)
{
	return true;
}

bool EntityManager::Load(pugi::xml_node& node)
{
	return true;
}

bool EntityManager::CleanUp()
{
	while(entities.size() != 0)
	{
		delete *entities.begin();
		entities.erase(entities.begin());
	}
	while(buffer.size() != 0)
	{
		delete* buffer.begin();
		buffer.erase(buffer.begin());
	}
	toDelete.erase(toDelete.begin(), toDelete.end());


	return true;
}

Entity* EntityManager::CreateEntity(EntityType type, fPoint position, bool flip, Player* parent)
{
	Entity* entity = nullptr;

	switch(type)
	{
	case EntityType::ENTITY:
		entity = new Entity(type, position, flip, parent);
		break;
	case EntityType::PLAYER:
		entity = new Player(position, flip);
		break;
	case EntityType::SHURIKEN:
		entity = new Shuriken(position, flip, parent);
		break;
	case EntityType::ENEMY:
		entity = new Enemy(type, position, flip, parent);
		break;
	case EntityType::BAT:
		entity = new Bat(position, flip, parent);
		break;
	case EntityType::GUARD:
		entity = new Guard(position, flip, parent);
		break;
	}

	buffer.push_back(entity);

	return entity;
}

void EntityManager::DeleteEntity(Entity* entity)
{
	entity->toDelete = true;
	entity = nullptr;
}

void EntityManager::FastDeleteEntity(vector<Entity*>::iterator itr)
{
	toDelete.push_back(itr);
}