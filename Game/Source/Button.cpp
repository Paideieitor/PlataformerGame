#include "Button.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "UIManager.h"

#include "Animation.h"
#include "Timer.h"

#define BUTTON_PRESS_COLDOWN 0.2f

Button::Button(const char* name, fPoint position, iPoint size, Observer* observer, int renderLayer)
	: Element(name, ElemType::BUTTON, position, { 105,28 }, observer, renderLayer), clicked(false)
{
	label = app->tex->Load(app->ui->buttonFont, name, labelSize);

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
	app->tex->UnLoad(label);

	delete normal;
	delete focused;
	delete pressed;
	delete disabled;
}

bool Button::Update(float dt, bool clickable)
{
	if(state != PRESSED && state != DISABLED)
	{
		state = NORMAL;
		if(clickable)
		{
			state = FOCUSED;
			if(app->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				state = PRESSED;
				clicked = true;
			}
		}
	}
	if(clicked)
	{
		if(!clickable)
			clicked = false;
		else if(app->input->GetMouseButtonDown(1) == KEY_UP)
		{
			observer->Callback(this, ElementData());
			clicked = false;
		}
	}

	switch(state)
	{
	case Element::NORMAL:
		currentAnimation = normal;
		break;
	case Element::FOCUSED:
		currentAnimation = focused;
		break;
	case Element::PRESSED:
		currentAnimation = pressed;
		if(!pressedColdown && !clicked)
		{
			pressedColdown = new Timer();
			pressedColdown->Start();
		}
		if(pressedColdown && pressedColdown->ReadSec() >= BUTTON_PRESS_COLDOWN)
		{
			delete pressedColdown;
			pressedColdown = nullptr;
			state = NORMAL;
		}
		break;
	case Element::DISABLED:
		currentAnimation = disabled;
		if(pressedColdown)
		{
			delete pressedColdown;
			pressedColdown = nullptr;
		}
		break;
	}

	app->render->SetTextureEvent(layer, texture, GetDrawPosition(), currentAnimation->GetFrame(dt), false, false);
	app->render->SetTextureEvent(layer, label, GetDrawPosition(&labelSize), { 0,0,labelSize.x, labelSize.y }, false, false);
	if(app->uiDebug)
		app->render->SetRectangleEvent(20, GetDrawPosition(), size, 255, 100, 100, 50, false);


	return true;
}

void Button::Rename(char* newName)
{
	app->tex->UnLoad(label);
	label = app->tex->Load(app->ui->buttonFont, newName, labelSize);
}
