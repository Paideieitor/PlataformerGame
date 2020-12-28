#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include "Element.h"
#include "ElemType.h"

class Animation;

class CheckBox : public Element
{
public:

	CheckBox(const char* name, fPoint position, bool selected , Observer* observer, int renderLayer);
	~CheckBox();

	bool Update(float dt, bool clickable);

private:

	bool selected = false;

	Animation* selectedFocused = nullptr;
};

#endif //__CHECKBOX_H__