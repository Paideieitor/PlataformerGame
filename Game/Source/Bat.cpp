#include "Input.h"
#include "DungeonScene.h"
#include "Pathfinding.h"
#include "Bat.h"
#include "Collider.h"

#include "Animation.h"

Bat::Bat(fPoint position, bool flip, Player* parent) : Enemy(EntityType::BAT, position, flip, parent)
{
	resting = true;

	pathColdown = 1.0f;
	pathTimer = 0.0f;

	texture = app->tex->Load("Assets/textures/bat.png");

	idle = new Animation(1);
	idle->PushBack(0, 0, 16, 16);

	fly = new Animation(2, true, 0.10f);
	fly->PushBack(16, 0, 16, 16);
	fly->PushBack(32, 0, 16, 16);

	currentAnimation = idle;
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
		if (path)
		{
			delete path;
			path = nullptr;
		}

		if(app->dungeonscene->player)
			if(abs(position.x - app->dungeonscene->player->position.x) < 100 && abs(position.y - app->dungeonscene->player->position.y) < 100)
			{
				if(currentAnimation != fly)
					currentAnimation = fly;
				path = app->paths->PathTo(position, app->dungeonscene->player->position);
			}
	}

	if(path)
	{
		if(app->pathDebug)
			path->DrawPath();
		bool end;
		fPoint destination = path->NextPoint(position, end);

		if(end)
		{
			delete path;
			path = nullptr;
		}
		else
		{
			if(resting)
				resting = false;
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

	if(app->pathDebug)
	{
		fPoint d = GetDrawPosition({ 200, 200 });
		app->render->SetRectangleEvent(10, d, { 200,200 }, 255, 0, 0, 255, true, false);
	}

	return true;
}