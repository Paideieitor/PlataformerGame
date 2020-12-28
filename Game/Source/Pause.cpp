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
	else if(element == (Element*)quitButton)
	{
		app->toSave = true;
		app->input->quit = true;
	}
}

void Pause::Activate()
{
	if (!active)
	{
		active = true;

		app->ui->DeactivateAll();

		backButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Resume", { 192,76 }, this, { 60 });
		optionsButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Settings", { 192,108 }, this, { 60 });
		mainMenuButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Back to Title", { 192,140 }, this, { 60 });
		quitButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Save and Quit", { 192,172 }, this, { 60 });
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
		app->ui->DeleteElement(quitButton);

		app->ui->ActivateAll();
	}
}
