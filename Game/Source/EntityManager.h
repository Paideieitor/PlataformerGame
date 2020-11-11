#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include "Module.h"
#include "Entities.h"

#include <vector>

class EntityManager : public Module
{

public:

	EntityManager();
	virtual ~EntityManager();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	bool CleanUp();

	Entity* CreateEntity(EntityType type, fPoint position, bool flip = false, Player* parent = nullptr);
	void DeleteEntity(Entity*);

	vector<Entity*> entities;

private:

	void FastDeleteEntity(vector<Entity*>::iterator);

	vector<Entity*> buffer;
	vector <vector<Entity*>::iterator> toDelete;
};

#endif