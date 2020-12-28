#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "Module.h"

class Button;
class CicleButton;
class CheckBox;
class SliderBar;

class Options : public Module
{
public:

	Options();
	virtual ~Options();

	bool Awake(pugi::xml_node&);

	bool Update(float dt);
	bool CleanUp();

	void UIEvent(Element*, ElementData&);

	void Activate();
	void Deactivate();

private:

	CicleButton* window = nullptr;
	int currentWindowOption = 0;
	CheckBox* borderless = nullptr;
	bool border = false;
	CheckBox* capFPS = nullptr;
	SliderBar* fxVolume = nullptr;
	SliderBar* musicVolume = nullptr;
	Button* backButton = nullptr;
};

#endif //__OPTIONS_H__