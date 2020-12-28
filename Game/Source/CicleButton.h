#ifndef __CICLEBUTTON_H__
#define __CICLEBUTTON_H__

#include "Element.h"
#include "ElemType.h"

class Button;

class CicleButton : public Element
{
public:

	CicleButton(const char* name, char** list, int size, int selected, fPoint position, Observer* observer, int renderLayer);
	~CicleButton();

	bool Update(float dt, bool clickable);

	void UIEvent(Element*, ElementData&);

private:

	char** list = nullptr;
	int size = 0;
	int current = 0;

	Button* cicleButton = nullptr;
	Button* applyButton = nullptr;
};

#endif //__CICLEBUTTON_H__