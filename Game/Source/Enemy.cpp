#include "Enemy.h"

#include "Collisions.h"

Enemy::Enemy(EntityType type, fPoint position, bool flip, Player* parent) : Entity(type, position, flip, parent)
{
	size = { 16,16 };

	pathColdown = 1.0f;
	pathTimer = 0.0f;

	body = app->collisions->CreateCollider(ColliderType::ENEMY, { (int)position.x,(int)position.y,size.x,size.y });
}

Enemy::~Enemy()
{
	app->collisions->DeleteCollider(body);
}

bool Enemy::Update(float dt)
{
	return true;
}