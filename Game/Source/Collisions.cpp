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
	CleanCheckers();

	int current = 0;

	bool loop = true;
	while(loop)
	{
		vector<Collider*>::iterator c = colliders.begin() + current;
		for(c; c != colliders.end(); c++)
		{
			Collider* collider = *c;
			if(collider->toDelete)
			{
				FastDeleteCollider(c);
				break;
			}
			else
				IterateCollider(collider);

			current++;
		}

		if(current == colliders.size())
			loop = false;
	}

	return true;
}

bool Collisions::PostUpdate()
{
	for(vector<Collider*>::iterator c = buffer.begin(); c != buffer.end(); c++)
	{
		colliders.push_back(*c);
		if((*c)->checker)
			checkers.push_back(*c);
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
	checkers.erase(checkers.begin(), checkers.end());

	colliders.shrink_to_fit();
	checkers.shrink_to_fit();
	buffer.shrink_to_fit();

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

void Collisions::FastDeleteCollider(vector<Collider*>::iterator coll)
{
	if ((*coll)->checker)
		for (vector<Collider*>::iterator ch = checkers.begin(); ch != checkers.end(); ch++)
			if (*ch == *coll)
				checkers.erase(ch);

	delete* coll;
	colliders.erase(coll);
}

void Collisions::IterateCollider(Collider* collider)
{
	bool loop = true;

	int current = 0;
	while(loop)
	{
		vector<Collider*>::iterator ch = checkers.begin() + current;
		for(ch; ch != checkers.end(); ch++)
		{
			Collider* checker = *ch;

			if(checker->checker)
			{
				if(checker != collider && !checker->toDelete && !collider->toDelete)
				{
					SDL_Rect r = checker->rect;
					int a = (checker->rect.x - checker->pastPosition.x);
					r.x = checker->pastPosition.x + ((checker->rect.x - checker->pastPosition.x) / 2);
					r.y = checker->pastPosition.y + ((checker->rect.y - checker->pastPosition.y) / 2);

					if (SDL_HasIntersection(&r, &collider->rect) == SDL_TRUE)
						checker->callback->Collision(checker, collider);
					else if (SDL_HasIntersection(&checker->rect, &collider->rect) == SDL_TRUE)
						checker->callback->Collision(checker, collider);
				}
			}
			else
			{
				checkers.erase(ch);
				break;
			}
			current++;
		}
		if(current == checkers.size())
			loop = false;
	}
	if(app->drawColliders)
		DrawCollider(collider);
}

void Collisions::CleanCheckers()
{
	int current = 0;

	bool loop = true;
	while (loop)
	{
		vector<Collider*>::iterator ch = checkers.begin() + current;
		for (ch; ch != checkers.end(); ch++)
		{
			Collider* check = *ch;
			if (!check->checker || check->toDelete)
			{
				checkers.erase(ch);
				break;
			}

			current++;
		}

		if (current == checkers.size())
			loop = false;
	}
}

void Collisions::DrawCollider(Collider* collider)
{
	SDL_Color color = { 255,255,255 };
	switch(collider->type)
	{
	case ColliderType::PLAYER:
		color = { 20,20,255 };
		break;
	case ColliderType::SHURIKEN:
		color = { 255,255,20 };
		break;
	case ColliderType::STATIC_SHURIKEN:
		color = { 100,255,20 };
		break;
	case ColliderType::ATTACK:
		color = { 255,20,20 };
		break;
	case ColliderType::CHECKPOINT:
		color = { 255,20,255 };
		break;
	case ColliderType::GROUND:
		color = { 20,255,20 };
		break;
	default:
		break;
	}
	app->render->SetRectangleEvent(5, { (float)collider->rect.x,(float)collider->rect.y }, { collider->rect.w,collider->rect.h }, color.r, color.g, color.b, 50);
}
