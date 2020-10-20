#include "Entities.h"
#include "EntityManager.h"
#include "Animation.h"
#include "Collisions.h"

#include "Input.h"

Player::Player(fPoint position)
{
	type = EntityType::PLAYER;
	this->position = position;
	flip = false;
	velocity = { 0,0 };
	wall = WallCollision::NONE;
	grounded = false;

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

	body = app->collisions->CreateCollider(ColliderType::PLAYER, { 0,0,10,13 }, true, this);
}

Player::~Player()
{
	app->tex->UnLoad(texture);

	delete idle;
	delete run;
	delete jump;

	app->collisions->DeleteCollider(body);
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
		}
	}
	else
	{
		velocity.y += timeOnAir;
		timeOnAir += dt;
		currentAnimation = jump;
	}

	if(app->input->GetKey(SDL_SCANCODE_W) == KEY_DOWN)
	{
		velocity.y = -4;
		timeOnAir = 0;
	}

	position.x += velocity.x;
	position.y += velocity.y;

	fPoint dPosition = GetDrawPosition(size);
	body->rect.x = (int)dPosition.x + 4;
	body->rect.y = (int)dPosition.y + 3;
	app->render->SetTextureEvent(5, texture, dPosition, currentAnimation->GetFrame(dt), flip);

	return true;
}

void Player::Collision(Collider* c1, Collider* c2)
{
	if (c1 == body && c2->type == ColliderType::GROUND)
	{
		fPoint dPosition = GetDrawPosition(size);
		if(dPosition.y < c2->rect.y)
		{
			grounded = true;
			position.y = c2->rect.y - size.y / 2 + 1;
			return;
		}
		if (dPosition.y + size.y / 2 > c2->rect.y + c2->rect.h)
		{
			position.y = c2->rect.y + c2->rect.h + size.y / 2;
			velocity.y = 0;
			return;
		}
		if(c2->rect.x > dPosition.x)
		{
			wall = WallCollision::LEFT;
			return;
		}
		if(c2->rect.x < dPosition.x)
		{
			wall = WallCollision::RIGHT;
			return;
		}
	}
}