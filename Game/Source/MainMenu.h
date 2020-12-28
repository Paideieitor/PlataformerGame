#ifndef __MAINMENU_H__
#define __MAINMENU_H__

#include "Module.h"

class Button;

struct SDL_Texture;

class MainMenu : public Module
{
public:

	MainMenu();
	virtual ~MainMenu();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

	void UIEvent(Element*, ElementData&);

private:

	bool change;

	Button* playButton = nullptr;
	Button* optionsButton = nullptr;
	Button* quitButton = nullptr;

	SDL_Texture* bern = nullptr;
};

#endif //__MAINMENU_H__