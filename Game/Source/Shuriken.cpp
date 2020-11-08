#include "Render.h"
#include "Animation.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "Entities.h"

#include "Log.h"

Shuriken::Shuriken(fPoint position, bool flip) : Entity(EntityType::SHURIKEN, position, flip)
{
	if(flip)
		velocity = -3;
	else
		velocity = 3;

	hit = false;

	size.x = 16;
	size.y = 16;

	texture = app->tex->Load("Assets/textures/shuriken.png");

	currentAnimation = new Animation(1, true, 0.25f);
	currentAnimation->PushBack(0, 0, 16, 16);

	body = app->collisions->CreateCollider(ColliderType::SHURIKEN, { (int)position.x, (int)position.y, size.x, size.y }, true, this);
	
	LOG("Shuriken body = %d, %d", (int)position.x, (int)position.y);
}

Shuriken::~Shuriken()
{
	app->tex->UnLoad(texture);

	delete currentAnimation;

	if(body)
		app->collisions->DeleteCollider(body);
	if(ground)
		app->collisions->DeleteCollider(ground);
}

bool Shuriken::Update(float dt)
{
	if(toRemove)
	{
		if(body)
		{
			app->collisions->DeleteCollider(body);
			body = nullptr;
		}
		if(ground)
		{
			app->collisions->DeleteCollider(ground);
			ground = nullptr;
		}
		return true;
	}

	if (hit)
		velocity = 0;

	fPoint dPosition = GetDrawPosition(size);
	app->render->SetTextureEvent(5, texture, dPosition, currentAnimation->GetFrame(dt), flip);

	position.x += velocity;

	dPosition = GetDrawPosition(size);
	body->SetPosition((int)dPosition.x, (int)dPosition.y);

	return true;
}

void Shuriken::Collision(Collider* c1, Collider* c2)
{
	if(!ground && c2->type == ColliderType::GROUND)
	{
		if(body->rect.y < c2->rect.y) {}
		else if (c2->rect.x > body->rect.x)
		{
			hit = true;
		}
		else if(c2->rect.x < body->rect.x)
		{
			hit = true;
		}
		if(hit)
		{
			body->checker = false;
			ground = app->collisions->CreateCollider(ColliderType::GROUND, body->rect);
		}
		return;
	}

	if(c2->type == ColliderType::SHURIKEN)
	{
		toRemove = true;
		for(vector<Entity*>::iterator e = app->entitymanager->entities.begin(); e != app->entitymanager->entities.end(); e++)
			if((*e)->body == c2)
			{
				(*e)->toRemove = true;
				break;
			}

		return;
	}
}