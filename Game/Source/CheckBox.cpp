#include "CheckBox.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "UIManager.h"

#include "Animation.h"

CheckBox::CheckBox(const char* name, fPoint position, bool selected, Observer* observer, int renderLayer)
	: Element(name, ElemType::CHECK_BOX, position, { 28,28 }, observer, renderLayer), selected(selected)
{
	label = app->tex->Load(app->ui->buttonFont, name, labelSize);

	normal = new Animation(1);
	normal->PushBack(0, 29, 28, 28);
	focused = new Animation(1);
	focused->PushBack(107, 29, 28, 28);
	pressed = new Animation(1);
	pressed->PushBack(214, 29, 28, 28);
	disabled = new Animation(1);
	disabled->PushBack(321, 29, 28, 28);
	selectedFocused = new Animation(1);
	selectedFocused->PushBack(428, 29, 28, 28);
}

CheckBox::~CheckBox()
{
	delete selectedFocused;
}

bool CheckBox::Update(float dt, bool clickable)
{
	if(state != DISABLED)
	{
		if(clickable)
			if(app->input->GetMouseButtonDown(1) == KEY_DOWN)
			{
				selected = !selected;
				observer->Callback(this, { nullptr, 0, selected });
			}

		if(selected)
			if(!clickable)
				state = PRESSED;
			else
				state = FOCUSED;
		else
			if(!clickable)
				state = NORMAL;
			else
				state = FOCUSED;
	}

	switch(state)
	{
	case Element::NORMAL:
		currentAnimation = normal;
		break;
	case Element::FOCUSED:
		if(!selected)
			currentAnimation = focused;
		else
			currentAnimation = selectedFocused;
		break;
	case Element::PRESSED:
		currentAnimation = pressed;
		break;
	case Element::DISABLED:
		currentAnimation = disabled;
		break;
	}

	app->render->SetTextureEvent(layer, texture, GetDrawPosition(), currentAnimation->GetFrame(dt), false, false);
	fPoint labelDraw = GetDrawPosition(&labelSize);
	labelDraw.x -= labelSize.x - 8;
	app->render->SetTextureEvent(layer, label, labelDraw, { 0,0,labelSize.x, labelSize.y }, false, false);
	if (app->uiDebug)
		app->render->SetRectangleEvent(20, GetDrawPosition(), size, 255, 100, 100, 50, false);

	return true;
}
