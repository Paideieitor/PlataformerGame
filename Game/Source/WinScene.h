#ifndef __WINSCENE_H__
#define __WINSCENE_H__

#include "Module.h"

class Button;

struct SDL_Texture;

class WinScene : public Module
{
public:

	WinScene();
	virtual ~WinScene();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	void UIEvent(Element*);

private:

	Button* mainMenuButton = nullptr;
	Button* quitButton = nullptr;

	SDL_Texture* win = nullptr;
};

#endif //__WINSCENE_H__