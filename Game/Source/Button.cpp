#include "Button.h"

#include "App.h"
#include "Input.h"
#include "Render.h"

Button::Button(const char* name, fPoint position, iPoint size, Observer* observer) : Element(name, ElemType::BUTTON, position, size, observer)
{
}

Button::~Button()
{
}

bool Button::Update(float dt, bool clickable)
{
	if(state != PRESSED || state != DISABLED)
		if (clickable)
		{
			state = FOCUSED;
			if (app->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				state = PRESSED;
				observer->Callback(this);
			}
		}

	switch (state)
	{
	case Element::NORMAL:
		currentAnimation = normal;
		break;
	case Element::FOCUSED:
		currentAnimation = focused;
		break;
	case Element::PRESSED:
		currentAnimation = pressed;
		break;
	case Element::DISABLED:
		currentAnimation = disabled;
		break;
	}

	app->render->SetRectangleEvent(20, GetDrawPosition(), size, 100, 100, 100, 255, false);

	return true;
}