#include "Render.h"
#include "Animation.h"
#include "Collisions.h"
#include "EntityManager.h"
#include "Entities.h"

#include "Log.h"

Shuriken::Shuriken(fPoint position, bool flip, Player* parent) : Entity(EntityType::SHURIKEN, position, flip)
{
	if(flip)
		velocity = -3;
	else
		velocity = 3;

	this->parent = parent;

	hit = false;
	wait = false;

	size.x = 16;
	size.y = 16;

	texture = app->tex->Load("Assets/textures/shuriken.png");

	currentAnimation = new Animation(1, true, 0.25f);
	currentAnimation->PushBack(0, 0, 16, 16);

	fPoint dPosition = GetDrawPosition(size);
	body = app->collisions->CreateCollider(ColliderType::SHURIKEN, { (int)dPosition.x, (int)dPosition.y, size.x, size.y }, true, this);
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
	if(hit)
	{
		velocity = 0;
		if(!wait)
		{
			body->type = ColliderType::STATIC_SHURIKEN;
			body->checker = false;
		}
	}
	wait = false;

	fPoint dPosition = GetDrawPosition(size);
	app->render->SetTextureEvent(5, texture, dPosition, currentAnimation->GetFrame(dt), flip);

	position.x += velocity;

	dPosition = GetDrawPosition(size);
	body->SetPosition((int)dPosition.x, (int)dPosition.y);
	//if(ground)
	//	ground->SetPosition((int)dPosition.x, (int)dPosition.y);

	return true;
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

	if(c2->type == ColliderType::STATIC_SHURIKEN)
	{
		toDelete = true;
		for(vector<Entity*>::iterator e = parent->shurikens.begin(); e != parent->shurikens.end(); e++)
			if((*e)->body == c2)
			{
				(*e)->toDelete = true;
				parent->shurikens.erase(e);
				break;
			}

		return;
	}

	if (c2->type == ColliderType::PLAYER)
		wait = true;
}