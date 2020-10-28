#ifndef __LOSESCENE_H__
#define __LOSESCENE_H__

#include "Module.h"

struct SDL_Texture;

class LoseScene : public Module
{
public:

	LoseScene();
	virtual ~LoseScene();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

private:

	SDL_Texture* lose = nullptr;
};

#endif //__LOSESCENE_H__
