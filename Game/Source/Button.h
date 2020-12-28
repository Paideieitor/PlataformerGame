#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Element.h"
#include "ElemType.h"

class Timer;

class Button : public Element
{
public:

	Button(const char* name, fPoint position, iPoint size, Observer* observer, int renderLayer);
	~Button();

	bool Update(float dt, bool clickable);

	void Rename(char* newName);

private:

	bool clicked;
	Timer* pressedColdown = nullptr;

	bool slider = false;

	friend class SliderBar;
};

#endif //__BUTTON_H__