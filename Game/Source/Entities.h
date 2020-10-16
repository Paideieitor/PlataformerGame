#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "Point.h"

enum class EntityType
{
	ENTITY,
	PLAYER
};

class Entity
{

public:

	Entity();
	virtual ~Entity();

	virtual bool Update(float dt);

	bool toDelete;

protected:

	EntityType type;

	fPoint position;
	iPoint size;

	fPoint GetDrawPosition(iPoint size);
	SDL_Texture* texture;
};

class Player : public Entity
{

public:

	Player(fPoint position);
	virtual ~Player();

	bool Update(float dt);
};