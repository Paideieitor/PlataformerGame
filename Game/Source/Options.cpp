#include "App.h"
#include "Input.h"
#include "Window.h"
#include "Render.h"
#include "Options.h"

#include "UIManager.h"
#include "Button.h"
#include "CicleButton.h"

Options::Options()
{
	name = "options";
}

Options::~Options()
{
}

bool Options::Awake(pugi::xml_node& node)
{
	currentWindowOption = node.attribute("window").as_int();

	return true;
}

bool Options::Update(float dt)
{
	app->render->SetRectangleEvent(70, { 0,0 }, { 384, 216 }, 0, 0, 0, 100, false);

	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		Deactivate();

	return true;
}

bool Options::CleanUp()
{
	Deactivate();

	return true;
}

void Options::UIEvent(Element* element, ElementData& data)
{
	if(element == (Element*)backButton)
		Deactivate();
	else if(element == (Element*)window)
	{
		currentWindowOption = data.current;
		pugi::xml_node node;
		pugi::xml_document* doc = app->GetConfig(node);
		node.child(name.c_str()).attribute("window").set_value(currentWindowOption);

		bool fullscreen = false;
		bool windowed = false;
		if (data.name == "Windowed")
		{
		}
		else if (data.name == "Windowed Fullscreen")
		{
			windowed = true;
		}
		else if (data.name == "Fullscreen")
		{
			fullscreen = true;
			windowed = true;
		}
		app->win->SetScreen(fullscreen, windowed);
		node.child("window").child("fullscreen").attribute("value").set_value(fullscreen);
		node.child("window").child("fullscreen_window").attribute("value").set_value(windowed);

		doc->save_file(app->configPath.c_str());
	}
}

void Options::Activate()
{
	if(!active)
	{
		active = true;

		app->ui->DeactivateAll();

		backButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Back", { 192, 108 }, this, { 80 });

		int size = 3;
		char** list = new char* [size];
		list[0] = "Windowed";
		list[1] = "Windowed Fullscreen";
		list[2] = "Fullscreen";
		if (currentWindowOption < 0 || currentWindowOption >= size)
			currentWindowOption = 0;
		window = (CicleButton*)app->ui->CreateElement(ElemType::CICLE_BUTTON, "Window", { 192,50 }, this, { 80, list, size, currentWindowOption });
	}
}

void Options::Deactivate()
{
	if (active)
	{
		active = false;

		app->ui->DeleteElement(backButton);
		app->ui->DeleteElement(window);

		app->ui->ActivateAll();
	}
}
