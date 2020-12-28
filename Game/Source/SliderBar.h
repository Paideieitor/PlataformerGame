#ifndef __SLIDERBAR_H__
#define __SLIDERBAR_H__

#include "Element.h"
#include "ElemType.h"

class Button;

class SliderBar : public Element
{
public:

	SliderBar(const char* name, float selected, fPoint position, Observer* observer, int renderLayer);
	~SliderBar();

	bool Update(float dt, bool clickable);

	void UIEvent(Element*, ElementData&);

private:

	float current = 0;

	SDL_Texture* value = nullptr;
	iPoint valueSize = { 0,0 };

	Button* button = nullptr;
};

#endif //__SLIDERBAR_H__