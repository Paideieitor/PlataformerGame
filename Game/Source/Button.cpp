#include "Button.h"

#include "App.h"
#include "Render.h"

Button::Button(const char* name, fPoint position, iPoint size, Observer* observer) : Element(name, Element::BUTTON, position, size, observer)
{
}

Button::~Button()
{
}

bool Button::Update(float dt)
{
	app->render->SetRectangleEvent(20, GetDrawPosition(), size, 100, 100, 100, 255, false);

	return true;
}
