#ifndef __OPTIONS_H__
#define __OPTIONS_H__

#include "Module.h"

class Button;
class CicleButton;

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
	Button* backButton = nullptr;
};

#endif //__OPTIONS_H__