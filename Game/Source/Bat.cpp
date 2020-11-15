#include "Input.h"
#include "DungeonScene.h"
#include "Pathfinding.h"
#include "Bat.h"
#include "Collider.h"

#include "Animation.h"

Bat::Bat(fPoint position, bool flip, Player* parent) : Enemy(EntityType::BAT, position, flip, parent)
{
	pathColdown = 1.0f;
	pathTimer = 0.0f;

	texture = app->tex->Load("Assets/textures/bat.png");

	idle = new Animation(1);
	idle->PushBack(0, 0, 16, 16);

	fly = new Animation(2, true, 0.10f);
	fly->PushBack(16, 0, 16, 16);
	fly->PushBack(32, 0, 16, 16);

	currentAnimation = fly;
}

Bat::~Bat()
{
	app->tex->UnLoad(texture);

	if(path)
		delete path;

	delete idle;
	delete fly;
}

bool Bat::Update(float dt)
{
	float speed = 25.0f * dt;

	pathTimer += dt;
	if(pathTimer >= pathColdown)
	{
		pathTimer = 0.0f;
		if(path)
			delete path;

		path = app->paths->PathTo(position, app->dungeonscene->player->position);
	}

	if(path)
	{
		bool end;
		fPoint destination = path->NextPoint(position, end);

		if(end)
		{
			delete path;
			path = nullptr;
		}
		else
		{
			if(position.x < destination.x)
			{
				position.x += speed;
				if(position.x > destination.x)
					position.x = destination.x;
				flip = false;
			}
			else if(position.x > destination.x)
			{
				position.x -= speed;
				if(position.x < destination.x)
					position.x = destination.x;
				flip = true;
			}
			if(position.y < destination.y)
			{
				position.y += speed;
				if(position.y > destination.y)
					position.y = destination.y;
			}
			else if(position.y > destination.y)
			{
				position.y -= speed;
				if(position.y < destination.y)
					position.y = destination.y;
			}
		}
	}

	fPoint dPosition = GetDrawPosition(size);
	body->SetPosition((int)dPosition.x, (int)dPosition.y);

	app->render->SetTextureEvent(5, texture, dPosition, currentAnimation->GetFrame(dt), flip);

	return true;
}