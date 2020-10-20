#include "Collisions.h"

Collisions::Collisions()
{
	name = "collisions";
}

Collisions::~Collisions()
{

}

bool Collisions::Awake(pugi::xml_node& node)
{
	return true;
}

bool Collisions::Start()
{
	return true;
}

bool Collisions::PreUpdate()
{
	return true;
}

bool Collisions::Update(float dt)
{
	for(vector<Collider*>::iterator c = colliders.begin(); c != colliders.end(); c++)
	{
		Collider* collider = *c;
		for(vector<Collider*>::iterator ch = checkers.begin(); ch != checkers.end(); ch++)
		{
			Collider* checker = *ch;
			if(checker != collider)
				if(SDL_HasIntersection(&checker->rect, &collider->rect) == SDL_TRUE)
					checker->callback->Collision(checker, collider);
		}
		app->render->SetRectangleEvent(5, { (float)collider->rect.x,(float)collider->rect.y }, { collider->rect.w,collider->rect.h }, 20, 255, 20, 50);
	}

	for(vector<vector<Collider*>::iterator>::iterator i = toDelete.begin(); i != toDelete.end(); i++)
	{
		delete **i;
		colliders.erase(*i);
	}
	toDelete.erase(toDelete.begin(), toDelete.end());

	return true;
}
bool Collisions::PostUpdate()
{
	for(vector<Collider*>::iterator e = buffer.begin(); e != buffer.end(); e++)
	{
		colliders.push_back(*e);
		if((*e)->checker)
			checkers.push_back(*e);
	}
	buffer.erase(buffer.begin(), buffer.end());

	return true;
}

bool Collisions::Save(pugi::xml_node& node)
{
	return true;
}

bool Collisions::Load(pugi::xml_node& node)
{
	return true;
}

bool Collisions::CleanUp()
{
	while(colliders.size() != 0)
	{
		delete *colliders.begin();
		colliders.erase(colliders.begin());
	}
	while(buffer.size() != 0)
	{
		delete *buffer.begin();
		buffer.erase(buffer.begin());
	}
	toDelete.erase(toDelete.begin(), toDelete.end());


	return true;
}

Collider* Collisions::CreateCollider(ColliderType type, SDL_Rect rect, bool checker, Entity* callback)
{
	Collider* collider = new Collider(type, rect, checker, callback);

	buffer.push_back(collider);

	return collider;
}

void Collisions::DeleteCollider(Collider* collider)
{
	collider->toDelete = true;
	collider = nullptr;
}

void Collisions::FastDeleteCollider(vector<Collider*>::iterator itr)
{
	toDelete.push_back(itr);
}