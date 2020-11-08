#include "EntityManager.h"
#include "Animation.h"
#include "Audio.h"
#include "Collisions.h"
#include "DungeonScene.h"
#include "Input.h"
#include "Entities.h"

#include "Log.h"

#define MAX_JUMPS 1

Player::Player(fPoint position, bool flip) : Entity(EntityType::PLAYER, position, flip)
{
	wall = WallCollision::NONE;

	grounded = false;

	jumps = 0;
	velocity = { 0,0 };
	timeOnAir = 0;

	shurikenColdown = 0.5f;
	shurikenTimer = 0.0f;

	size.x = 16;
	size.y = 16;

	texture = app->tex->Load("Assets/textures/ninja.png");

	idle = new Animation(2, true, 0.25f);
	idle->PushBack(0, 0, 16, 16);
	idle->PushBack(16, 0, 16, 16);

	run = new Animation(4, true, 0.1f);
	run->PushBack(16, 16, 16, 16);
	run->PushBack(0, 16, 16, 16);
	run->PushBack(32, 16, 16, 16);
	run->PushBack(0, 16, 16, 16);

	jump = new Animation(1, false);
	jump->PushBack(48, 16, 16, 16);

	currentAnimation = idle;

	feet = app->collisions->CreateCollider(ColliderType::PLAYER, { (int)position.x,(int)position.y,1,13 }, true, this);
	body = app->collisions->CreateCollider(ColliderType::PLAYER, { (int)position.x,(int)position.y,16,13 }, true, this);
}

Player::~Player()
{
	for(uint i = 0; i < shurikens.size(); i++)
		shurikens[i]->toDelete = true;

	app->tex->UnLoad(texture);

	delete idle;
	delete run;
	delete jump;

	app->collisions->DeleteCollider(body);
	app->collisions->DeleteCollider(feet);
}

bool Player::Update(float dt)
{
	currentAnimation = idle;

	if(shurikenTimer != 0.0f)
	{
		shurikenTimer += dt;
		if(shurikenTimer >= shurikenColdown)
			shurikenTimer = 0.0f;
	}

	if(shurikenTimer == 0.0f && app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_DOWN)
	{
		shurikens.push_back(app->entitymanager->CreateEntity(EntityType::SHURIKEN, position, flip));
		shurikenTimer += dt;
	}

	velocity.x = 0;
	if(wall != WallCollision::RIGHT && app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		velocity.x = -dt * 100;
		currentAnimation = run;
		flip = true;
	}
	if(wall != WallCollision::LEFT && app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		velocity.x = dt * 100;
		currentAnimation = run;
		flip = false;
	}
	wall = WallCollision::NONE;

	if(app->godMode)
	{
		velocity.y = 0;
		if(app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
			velocity.y = -dt * 100;
		if(app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
			velocity.y = dt * 100;
	}
	else
	{
		bool onGround = grounded;
		grounded = false;
		if(onGround)
		{
			if(velocity.y > 0)
			{
				velocity.y = 0;
				timeOnAir = 0;
				jumps = 0;
			}
		}
		else
		{
			velocity.y += timeOnAir * dt;
			timeOnAir += 5000.0f * dt;
			currentAnimation = jump;
		}

		if(jumps < MAX_JUMPS && app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
		{
			velocity.y = -200;
			jumps++;
			timeOnAir = 0;
			app->audio->PlayFx(app->dungeonscene->jumpSound);
		}
	}

	fPoint dPosition = GetDrawPosition(size);
	app->render->SetTextureEvent(5, texture, dPosition, currentAnimation->GetFrame(dt), flip);

	iPoint cameraOnPlayer = { (int)position.x - app->render->resolution.x / 2 , (int)position.y - app->render->resolution.y / 2 };
	if (cameraOnPlayer.y < 0)
		cameraOnPlayer.y = 0;
	app->render->camera.x = -cameraOnPlayer.x;
	app->render->camera.y = -cameraOnPlayer.y;

	position.x += velocity.x;
	if(!app->godMode)
		position.y += velocity.y * dt;
	else
		position.y += velocity.y;

	dPosition = GetDrawPosition(size);
	body->SetPosition((int)dPosition.x, (int)dPosition.y + 3);
	feet->SetPosition((int)dPosition.x + 8, (int)dPosition.y + 3);

	return true;
}

void Player::Collision(Collider* c1, Collider* c2)
{
	if(c1 == body && c2->type == ColliderType::GROUND)
	{
		if(feet->rect.y + feet->rect.h * 0.8 < c2->rect.y) {}
		else if(c2->rect.x > body->rect.x)
		{
			wall = WallCollision::LEFT;
		}
		else if(c2->rect.x < body->rect.x)
		{
			wall = WallCollision::RIGHT;
		}
		return;
	}
	if(c1 == feet && c2->type == ColliderType::GROUND)
	{
		if(feet->rect.y < c2->rect.y || (feet->rect.y > c2->rect.y && feet->rect.y + feet->rect.h < c2->rect.y + c2->rect.h))
		{
			grounded = true;
			position.y = c2->rect.y - feet->rect.h / 2;
			return;
		}
		else if(feet->rect.y + feet->rect.h > c2->rect.y + c2->rect.h)
		{
			position.y = c2->rect.y + c2->rect.h + size.y / 2;
			velocity.y = 0;
		}
		else
		{
			grounded = true;
			position.y = c2->rect.y - feet->rect.h / 2;
			return;
		}
	}

	if(c1 == body && c2->type == ColliderType::CHECKPOINT)
	{
		app->dungeonscene->iterate = true;
	}
	if(!app->godMode && c1 == body && c2->type == ColliderType::ATTACK)
	{
		app->audio->PlayFx(app->dungeonscene->deathSound);
		app->dungeonscene->RespawnPlayer();
	}
}