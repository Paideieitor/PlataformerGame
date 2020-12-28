#ifndef __PAUSE_H__
#define __PAUSE_H__

#include "Module.h"

class Button;

class Pause : public Module
{
public:

	Pause();
	virtual ~Pause();

	bool Update(float dt);
	bool CleanUp();

	void UIEvent(Element*, ElementData&);

	void Activate();
	void Deactivate();

private:

	Button* mainMenuButton = nullptr;
	Button* optionsButton = nullptr;
	Button* backButton = nullptr;

	bool waiting = false;
};

#endif //__PAUSE_H__