#ifndef __COLLISION_H__
#define __COLLISION_H__

#include "Module.h"
#include "Collider.h"

#include <vector>

class Collisions : public Module
{

public:

	Collisions();
	virtual ~Collisions();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	bool CleanUp();

	Collider* CreateCollider(ColliderType type, SDL_Rect rect, bool checker = false, Entity* callback = nullptr);
	void DeleteCollider(Collider*);

private:

	void FastDeleteCollider(vector<Collider*>::iterator);
	void IterateCollider(Collider* collider);
	void CleanCheckers();

	vector<Collider*> colliders;
	vector<Collider*> checkers;
	vector<Collider*> buffer;

	void DrawCollider(Collider*);
};

#endif // __COLLISION_H__