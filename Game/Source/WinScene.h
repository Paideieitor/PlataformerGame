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

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	bool CleanUp();

	void UIEvent(Element*, ElementData&);

	void SendTimerResults(uint* results, uint size);

private:

	void SetHighScore(int total);

	bool newRecord;
	iPoint rSize;
	SDL_Texture* record = nullptr;

	iPoint ySSize;
	SDL_Texture* yourScore = nullptr;
	uint* timerResults = nullptr;
	uint timerSize;
	SDL_Texture** timerTextures = nullptr;
	SDL_Texture* totalTexture = nullptr;

	int highTotal;
	iPoint hSSize;
	SDL_Texture* higestScore = nullptr;
	uint* highResults = nullptr;
	uint highSize;
	SDL_Texture** highTextures = nullptr;
	SDL_Texture* totalHighTexture = nullptr;

	Button* mainMenuButton = nullptr;
	Button* quitButton = nullptr;

	SDL_Texture* win = nullptr;
};

#endif //__WINSCENE_H__