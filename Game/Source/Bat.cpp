#include "Entities.h"

#include "Animation.h"

Bat::Bat(fPoint position, bool flip, Player* parent) : Enemy(EntityType::BAT, position, flip, parent)
{
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

	delete idle;
	delete fly;
}

bool Bat::Update(float dt)
{
	app->render->SetTextureEvent(5, texture, position, currentAnimation->GetFrame(dt), flip);

	return true;
}