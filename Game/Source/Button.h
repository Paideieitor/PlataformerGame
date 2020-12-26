#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Element.h"
#include "ElemType.h"

class Timer;

class Button : public Element
{
public:

	Button(const char* name, fPoint position, iPoint size, Observer* observer);
	~Button();

	bool Update(float dt, bool clickable);

private:

	Timer* pressedColdown = nullptr;
};

#endif //__BUTTON_H__