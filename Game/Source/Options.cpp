#include "App.h"
#include "Input.h"
#include "Audio.h"
#include "Window.h"
#include "Render.h"
#include "Options.h"

#include "UIManager.h"
#include "Button.h"
#include "CicleButton.h"
#include "CheckBox.h"
#include "SliderBar.h"

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

	pugi::xml_node cNode;
	pugi::xml_document* doc = app->GetConfig(cNode);
	border = cNode.child("window").child("borderless").attribute("value").as_bool();

	return true;
}

bool Options::Update(float dt)
{
	app->render->SetRectangleEvent(70, { 0,0 }, { 384, 216 }, 20, 50, 20, 255, false);

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
	pugi::xml_node node;
	pugi::xml_document* doc = app->GetConfig(node);

	if(element == (Element*)backButton)
		Deactivate();
	else if(element == (Element*)window)
	{
		currentWindowOption = data.current;
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
	}
	else if (element == (Element*)borderless)
	{
		border = data.checked;

		app->win->SetScreen(border);
		node.child("window").child("borderless").attribute("value").set_value(border);
	}
	else if(element == (Element*)capFPS)
	{
		app->capped = data.checked;
		node.child("app").child("fps").attribute("capped").set_value(app->capped);
	}
	else if(element == (Element*)fxVolume)
		app->audio->SetFxVolume(data.current);
	else if(element == (Element*)musicVolume)
		app->audio->SetMusicVolume(data.current);

	doc->save_file(app->configPath.c_str());
}

void Options::Activate()
{
	if(!active)
	{
		active = true;

		app->ui->DeactivateAll();

		backButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Back", { 192, 200 }, this, { 80 });

		int size = 3;
		char** list = new char* [size];
		list[0] = "Windowed";
		list[1] = "Windowed Fullscreen";
		list[2] = "Fullscreen";
		if (currentWindowOption < 0 || currentWindowOption >= size)
			currentWindowOption = 0;
		window = (CicleButton*)app->ui->CreateElement(ElemType::CICLE_BUTTON, "Window", { 192,70 }, this, { 80, list, size, currentWindowOption });

		borderless = (CheckBox*)app->ui->CreateElement(ElemType::CHECK_BOX, "Borderless", { 192,128 }, this, { 80, nullptr, 0, 0, border });
		capFPS = (CheckBox*)app->ui->CreateElement(ElemType::CHECK_BOX, "Cap FPS", { 300,128 }, this, { 80, nullptr, 0, 0, app->capped });

		fxVolume = (SliderBar*)app->ui->CreateElement(ElemType::SLIDER, "Fx", { 200,20 }, this, { 80, nullptr, app->audio->GetFxVolume() });
		musicVolume = (SliderBar*)app->ui->CreateElement(ElemType::SLIDER, "Music", { 200,50 }, this, { 80, nullptr, app->audio->GetMusicVolume() });
	}
}

void Options::Deactivate()
{
	if (active)
	{
		active = false;

		app->ui->DeleteElement(backButton);
		app->ui->DeleteElement(window);
		app->ui->DeleteElement(borderless);
		app->ui->DeleteElement(capFPS);
		app->ui->DeleteElement(fxVolume);
		app->ui->DeleteElement(musicVolume);

		app->ui->ActivateAll();
	}
}
