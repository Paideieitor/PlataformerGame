#include "Render.h"
#include "Animation.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "Player.h"
#include "Shuriken.h"

Shuriken::Shuriken(fPoint position, bool flip, Player* parent) : Entity(EntityType::SHURIKEN, position, flip, parent)
{
	if(flip)
		velocity = -250;
	else
		velocity = 250;

	hit = false;
	wait = false;

	size.x = 16;
	size.y = 16;

	texture = app->tex->Load("Assets/textures/shuriken.png");

	currentAnimation = new Animation(2, true, 0.10f);
	currentAnimation->PushBack(0, 0, 16, 16);
	currentAnimation->PushBack(16, 0, 16, 16);

	fPoint dPosition = GetDrawPosition(size);
	body = app->collisions->CreateCollider(ColliderType::SHURIKEN, { (int)dPosition.x, (int)dPosition.y, size.x, size.y }, true, this);
}

Shuriken::~Shuriken()
{
	app->tex->UnLoad(texture);

	delete currentAnimation;

	if(body)
		app->collisions->DeleteCollider(body);
}

bool Shuriken::Update(float dt)
{
	if(hit)
	{
		velocity = 0;
		currentAnimation->Pause();
		if(!wait)
		{
			body->type = ColliderType::STATIC_SHURIKEN;
			body->checker = false;
		}
	}
	wait = false;

	fPoint dPosition = GetDrawPosition(size);
	app->render->SetTextureEvent(5, texture, dPosition, currentAnimation->GetFrame(dt), flip);

	position.x += velocity * dt;

	dPosition = GetDrawPosition(size);
	body->SetPosition((int)dPosition.x, (int)dPosition.y);

	return true;
}

bool InteractCollider(ColliderType type)
{
	if (type == ColliderType::STATIC_SHURIKEN || type == ColliderType::ENEMY)
		return true;
	return false;
}

void Shuriken::Collision(Collider* c1, Collider* c2)
{
	if(!hit && c2->type == ColliderType::GROUND)
	{
		if(body->rect.y + body->rect.h * 0.5 < c2->rect.y) {}
		else if (c2->rect.x > body->rect.x)
		{
			hit = true;
		}
		else if(c2->rect.x < body->rect.x)
		{
			hit = true;
		}

		return;
	}

	if(InteractCollider(c2->type))
	{
		toDelete = true;

		switch (c2->type)
		{
		case ColliderType::STATIC_SHURIKEN:
			for (vector<Entity*>::iterator e = parent->shurikens.begin(); e != parent->shurikens.end(); e++)
				if ((*e)->body == c2)
				{
					(*e)->toDelete = true;
					parent->shurikens.erase(e);
					break;
				}
			break;
		case ColliderType::ENEMY:
			for (vector<Entity*>::iterator e = app->entitymanager->entities.begin(); e != app->entitymanager->entities.end(); e++)
				if ((*e)->body == c2)
				{
					(*e)->toDelete = true;
					break;
				}
			break;
		}
	}

	if (c2->type == ColliderType::PLAYER)
		wait = true;
}