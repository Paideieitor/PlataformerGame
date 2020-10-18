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

	Entity* CreateEntity(EntityType type, fPoint position);
	void DeleteEntity(Entity*);

private:

	void FastDeleteEntity(vector<Entity*>::iterator);

	vector<Entity*> entities;
	vector<Entity*> buffer;
	vector <vector<Entity*>::iterator> toDelete;
};

#endif