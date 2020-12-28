#include "App.h"
#include "Input.h"
#include "Render.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "DungeonScene.h"
#include "Options.h"
#include "Pause.h"

#include "UIManager.h"
#include "Button.h"

Pause::Pause()
{
	name = "pause";
}

Pause::~Pause()
{
}

bool Pause::Update(float dt)
{
	app->render->SetRectangleEvent(50, { 0,0 }, { 384, 216 }, 0, 0, 0, 100, false);

	if(waiting && !app->options->active)
	{
		waiting = false;

		Deactivate();
		Activate();
	}

	return true;
}

bool Pause::CleanUp()
{
	Deactivate();

	return true;
}

void Pause::UIEvent(Element* element, ElementData&)
{
	if(element == (Element*)backButton)
		Deactivate();
	else if(element == (Element*)mainMenuButton)
	{
		app->toSave = true;
		Deactivate();
		app->fade->ChangeScene(app->dungeonscene, app->mainmenu);
	}
	else if(element == (Element*)optionsButton)
	{
		app->options->Activate();
		waiting = true;
	}
}

void Pause::Activate()
{
	if (!active)
	{
		active = true;

		app->ui->DeactivateAll();

		optionsButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Options", { 192,76 }, this, { 60 });
		mainMenuButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Save and Exit", { 192,108 }, this, { 60 });
		backButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Back", { 192,140 }, this, { 60 });
	}
}

void Pause::Deactivate()
{
	if (active)
	{
		active = false;

		app->ui->DeleteElement(mainMenuButton);
		app->ui->DeleteElement(optionsButton);
		app->ui->DeleteElement(backButton);

		app->ui->ActivateAll();
	}
}
