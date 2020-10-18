#include "Entities.h"
#include "EntityManager.h"

#include "Input.h"

Player::Player(fPoint position)
{
	type = EntityType::PLAYER;
	this->position = position;

	texture = app->tex->Load("Assets/textures/BERN.png");
	SDL_QueryTexture(texture, NULL, NULL, &size.x, &size.y);
}

Player::~Player()
{
	app->tex->UnLoad(texture);
}

bool Player::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT)
	{
		position.y--;
	}
	if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT)
	{
		position.y++;
	}
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x--;
	}
	if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x++;
	}

	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
		app->entitymanager->CreateEntity(type, { 0,0 });

	app->render->SetTextureEvent(5, texture, GetDrawPosition(size));

	return true;
}