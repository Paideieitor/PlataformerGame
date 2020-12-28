#include "SliderBar.h"

#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"

#include "UIManager.h"
#include "Button.h"

#include "Animation.h"

SliderBar::SliderBar(const char* name, float selected, fPoint position, Observer* observer, int renderLayer)
	: Element(name, ElemType::BUTTON, position, { 105,8 }, observer, renderLayer), current(selected)
{
	label = app->tex->Load(app->ui->buttonFont, name, labelSize);

	normal = new Animation(1);
	normal->PushBack(0, 58, 105, 8);
	focused = new Animation(1);
	focused->PushBack(107, 58, 105, 8);
	pressed = new Animation(1);
	pressed->PushBack(214, 58, 105, 8);
	disabled = new Animation(1);
	disabled->PushBack(321, 58, 105, 8);

	if (current < 0)
		current = 0;
	else if (current > 100)
		current = 100;

	char* valueChar = new char[6];
	sprintf(valueChar, "%d", (int)selected);
	value = app->tex->Load(app->ui->buttonFont, valueChar, valueSize);
	delete[] valueChar;
}

SliderBar::~SliderBar()
{
	app->ui->DeleteElement(button);

	app->tex->UnLoad(value);
}

bool SliderBar::Update(float dt, bool clickable)
{
	if(!button)
	{
		fPoint buttonPosition = { (GetDrawPosition().x + current * (105 / 100)) - 12 / 2, GetDrawPosition().y - 19 / 4 };
		buttonPosition.x += 6;
		buttonPosition.y += 8;
		button = (Button*)app->ui->CreateElement(ElemType::BUTTON, " ", buttonPosition, this, { layer + 1 });
		delete button->normal;
		delete button->focused;
		delete button->pressed;
		delete button->disabled;
		button->SetSize({ 12,19 });
		button->normal = new Animation(1);
		button->normal->PushBack(0, 87, 12, 19);
		button->focused = new Animation(1);
		button->focused->PushBack(107, 87, 12, 19);
		button->pressed = new Animation(1);
		button->pressed->PushBack(214, 87, 12, 19);
		button->disabled = new Animation(1);
		button->disabled->PushBack(321, 87, 12, 19);
		button->slider = true;
	}

	if(button->clicked || (clickable && app->input->GetMouseButtonDown(1) == KEY_DOWN))
	{
		if(app->input->GetMouseButtonDown(1) == KEY_DOWN)
		{
			button->state = PRESSED;
			button->clicked = true;
		}
		float max = (GetDrawPosition().x + 100 * (105 / 100)) - 12 / 2;
		max += 11;
		float min = (GetDrawPosition().x + 0 * (105 / 100)) - 12 / 2;
		min += 6;

		int newX;
		int a;
		app->input->GetMousePosition(newX, a);
		if(newX < min)
			newX = min;
		else if(newX > max)
			newX = max;

		button->position.x = newX;

		float pastvalue = current;
		current = ((newX - GetDrawPosition().x) / size.x) * 100;

		if(pastvalue != current)
		{
			char* valueChar = new char[6];
			sprintf(valueChar, "%d", (int)current);
			value = app->tex->Load(app->ui->buttonFont, valueChar, valueSize);
			delete[] valueChar;
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
		break;
	case Element::DISABLED:
		currentAnimation = disabled;
		break;
	}

	app->render->SetTextureEvent(layer, texture, GetDrawPosition(), currentAnimation->GetFrame(dt), false, false);
	fPoint labelDraw = GetDrawPosition(&labelSize);
	labelDraw.x -= labelSize.x + 50;
	app->render->SetTextureEvent(layer, label, labelDraw, { 0,0,labelSize.x, labelSize.y }, false, false);
	labelDraw = GetDrawPosition(&valueSize);
	labelDraw.x += valueSize.x + 48;
	app->render->SetTextureEvent(layer, value, labelDraw, { 0,0,valueSize.x, valueSize.y }, false, false);
	if (app->uiDebug)
		app->render->SetRectangleEvent(1000, GetDrawPosition(), size, 255, 100, 100, 50, false);

    return true;
}

void SliderBar::UIEvent(Element* element, ElementData&)
{
	if(element == (Element*)button)
		observer->Callback(this, { nullptr, (int)current });
}
