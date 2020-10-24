#include "Entities.h"
#include "EntityManager.h"
#include "Animation.h"
#include "Collisions.h"
#include "DungeonScene.h"

#include "Input.h"

#define MAXJUMPS 2

Player::Player(fPoint position)
{
	type = EntityType::PLAYER;
	this->position = position;
	flip = false;
	velocity = { 0,0 };
	wall = WallCollision::NONE;
	grounded = false;
	jumps = 0;

	texture = app->tex->Load("Assets/textures/ninja.png");

	size.x = 16;
	size.y = 16;

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

	body = app->collisions->CreateCollider(ColliderType::PLAYER, { 0,0,16,13 }, true, this);
	feet = app->collisions->CreateCollider(ColliderType::PLAYER, { 0,0,4,4 }, true, this);
}

Player::~Player()
{
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
		velocity.y += timeOnAir;
		timeOnAir += dt * 1.5f;
		currentAnimation = jump;
	}

	if(jumps < MAXJUMPS && app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		velocity.y = dt * -250;
		jumps++;
		timeOnAir = 0;
	}

	position.x += velocity.x;
	position.y += velocity.y;

	fPoint dPosition = GetDrawPosition(size);
	body->rect.x = (int)dPosition.x;
	body->rect.y = (int)dPosition.y + 3;
	feet->rect.x = (int)dPosition.x + 6;
	feet->rect.y = (int)dPosition.y + 12;
	app->render->SetTextureEvent(5, texture, dPosition, currentAnimation->GetFrame(dt), flip);

	return true;
}

void Player::Collision(Collider* c1, Collider* c2)
{
	if (c1 == body && c2->type == ColliderType::GROUND)
	{
		fPoint dPosition = GetDrawPosition(size);
		if (dPosition.y + size.y - 3 < c2->rect.y) {}
		else if (dPosition.y + size.y / 2 > c2->rect.y + c2->rect.h)
		{
			position.y = c2->rect.y + c2->rect.h + size.y / 2;
			velocity.y = 0;
		}
		else if(c2->rect.x > dPosition.x)
		{
			wall = WallCollision::LEFT;
		}
		else if(c2->rect.x < dPosition.x)
		{
			wall = WallCollision::RIGHT;
		}
		return;
	}
	if (c1 == feet && c2->type == ColliderType::GROUND)
	{
		fPoint dPosition = GetDrawPosition(size);
		if (dPosition.y < c2->rect.y)
		{
			grounded = true;
			position.y = c2->rect.y - size.y / 2 + 1;
			return;
		}
	}

	if (c1 == body && c2->type == ColliderType::CHECKPOINT)
	{
		app->dungeonscene->IterateCheckpoint();
	}
	if (c1 == body && c2->type == ColliderType::ATTACK)
	{
		app->dungeonscene->RespawnPlayer();
	}
}