#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "Element.h"

class Button : public Element
{
public:

	Button(const char* name, fPoint position, iPoint size, Observer* observer);
	~Button();

	bool Update(float dt);

private:

};

#endif //__BUTTON_H__