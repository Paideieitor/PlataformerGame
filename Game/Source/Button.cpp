#include "Button.h"

#include "App.h"
#include "Input.h"
#include "Render.h"

#include "Animation.h"
#include "Timer.h"

#define BUTTON_PRESS_COLDOWN 0.5f

Button::Button(const char* name, fPoint position, iPoint size, Observer* observer) : Element(name, ElemType::BUTTON, position, { 105,28 }, observer)
{
	normal = new Animation(1);
	normal->PushBack(0, 0, 105, 28);
	focused = new Animation(1);
	focused->PushBack(107, 0, 105, 28);
	pressed = new Animation(1);
	pressed->PushBack(214, 0, 105, 28);
	disabled = new Animation(1);
	disabled->PushBack(321, 0, 105, 28);

	currentAnimation = normal;
}

Button::~Button()
{
	delete normal;
	delete focused;
	delete pressed;
	delete disabled;
}

bool Button::Update(float dt, bool clickable)
{
	if (state != PRESSED && state != DISABLED)
	{
		state = NORMAL;
		if (clickable)
		{
			state = FOCUSED;
			if (app->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				state = PRESSED;
				observer->Callback(this);
			}
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
		if (!pressedColdown)
		{
			pressedColdown = new Timer();
			pressedColdown->Start();
		}
		if (pressedColdown->ReadSec() >= BUTTON_PRESS_COLDOWN)
		{
			delete pressedColdown;
			pressedColdown = nullptr;
			state = NORMAL;
		}
		break;
	case Element::DISABLED:
		currentAnimation = disabled;
		break;
	}

	app->render->SetTextureEvent(20, texture, GetDrawPosition(), currentAnimation->GetFrame(dt), false, false);
	if(app->uiDebug)
		app->render->SetRectangleEvent(20, GetDrawPosition(), size, 255, 100, 100, 50, false);


	return true;
}