#ifndef __LOGOSCENE_H__
#define __LOGOSCENE_H__

#include "Module.h"

struct SDL_Texture;

class LogoScene : public Module
{
public:

	LogoScene();
	virtual ~LogoScene();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

private:

	SDL_Texture* logo = nullptr;

	float aTimer;
	float timer;
	bool stop;
};

#endif // __SCENE_H__