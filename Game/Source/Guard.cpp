#include "Input.h"
#include "DungeonScene.h"
#include "Pathfinding.h"
#include "Guard.h"
#include "Collider.h"

#include "Animation.h"

Guard::Guard(fPoint position, bool flip, Player* parent) : Enemy(EntityType::GUARD, position, flip, parent)
{
	resting = false;

	chasing = false;

	fPoint velocity = { 0.0f,0.0f };

	texture = app->tex->Load("Assets/textures/guard.png");

	walk = new Animation(4, true, 0.3f);
	walk->PushBack(0, 0, 16, 16);
	walk->PushBack(16, 0, 16, 16);
	walk->PushBack(32, 0, 16, 16);
	walk->PushBack(16, 0, 16, 16);

	currentAnimation = walk;
}

Guard::~Guard()
{
	app->tex->UnLoad(texture);

	if (path)
		delete path;

	delete walk;
}

bool GuardDecoder(int id)
{
	if (id == 2)
		return true;
	return false;
}

bool ChaseDecoder(int id)
{
	if (id == 2 || id == 3)
		return true;
	return false;
}

bool Guard::Update(float dt)
{
	float speed = 25.0f * dt;

	iPoint visionSize = { 100,100 };
	fPoint visionPos = { position.x, position.y - visionSize.y / 2 };
	if (flip)
		visionPos.x = visionPos.x - visionSize.x;

	if (app->dungeonscene->player)
	{
		if(visionPos.x <= app->dungeonscene->player->position.x && visionPos.x + visionSize.x >= app->dungeonscene->player->position.x &&
			visionPos.y <= app->dungeonscene->player->position.y && visionPos.y + visionSize.y >= app->dungeonscene->player->position.y)
		{
			if(!chasing)
			{
				delete path;
				path = nullptr;
			}
			if(!path)
				path = app->paths->PathTo(position, app->dungeonscene->player->position, ChaseDecoder, true);

			chasing = true;
		}
		else
		{
			if(!path)
			{
				float distance = 50.0f;
				if(flip)
					distance = distance * -1;
				path = app->paths->PathTo(position, { position.x - distance, position.y }, GuardDecoder, true);
			}
		}

		velocity.x = 0.0f;
		if (path)
		{
			if (app->pathDebug)
				path->DrawPath();
			bool end;
			fPoint destination = path->NextPoint(position, end);

			if(end)
			{
				delete path;
				path = nullptr;
				flip = !flip;
			}
			else
			{
				if(!chasing)
				{
					Walk(speed, destination.x);
				}
				else
				{ 
					MoveType type = MoveType::NONE;
					fPoint specialDest = GetSpecialDestination(type);

					switch (type)
					{
					case MoveType::NONE:
						Walk(speed * 2, destination.x);
						break;
					case MoveType::JUMP:
						break;
					case MoveType::FALL:
						break;
					}
				}
			}
		}
	}

	position.x += velocity.x;
	position.y += velocity.y * dt;

	fPoint dPosition = GetDrawPosition(size);
	body->SetPosition((int)dPosition.x, (int)dPosition.y);

	app->render->SetTextureEvent(5, texture, dPosition, currentAnimation->GetFrame(dt), flip);

	if(app->pathDebug)
	{
		app->render->SetRectangleEvent(10, visionPos, visionSize, 255, 0, 0, 255, true, false);
	}

	return true;
}

fPoint* GetPathInfo(const Path& path, int& size, int& current)
{
	size = path.size;
	current = path.current;

	return path.path;
}

void Guard::Walk(float speed, float destination)
{
	if (position.x < destination)
	{
		velocity.x = speed;
		if (position.x + velocity.x > destination)
			velocity.x = destination - position.x;
		flip = false;
	}
	else if (position.x > destination)
	{
		velocity.x = -speed;
		if (position.x + velocity.x < destination)
			velocity.x = -(position.x - destination);
		flip = true;
	}
}

fPoint Guard::GetSpecialDestination(MoveType& type)
{
	int size = 0;
	int current = 0;
	fPoint* points = GetPathInfo(*path, size, current);

	if(!points)
		return position;

	if(current + 1 == size)
		return points[current];

	if (points[current].y < points[current + 1].y || position.x < points[current].y) //FALL 
	{
		type = MoveType::FALL;
		for (uint i = current; i < size; i++)
		{
			if (i + 1 != size)
			{
				if (points[i + 1].y <= points[i].y)
					return points[i];
				continue;
			}
			return points[i];
		}
	}
	else if (points[current].y > points[current + 1].y || position.x > points[current].y) //JUMP
	{
		type = MoveType::JUMP;
		for (uint i = current; i < size; i++)
		{
			if (i + 1 != size)
			{
				if (points[i + 1].y >= points[i].y)
					return points[i];
				continue;
			}
			return points[i];
		}
	}

	return points[current];
}
