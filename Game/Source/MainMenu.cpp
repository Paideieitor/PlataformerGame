#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "DungeonScene.h"
#include "Options.h"
#include "MainMenu.h"

#include "UIManager.h"
#include "Button.h"

#include "Defs.h"
#include "Log.h"

MainMenu::MainMenu() : Module()
{
	name = "mainmenu";
}

MainMenu::~MainMenu()
{

}

bool MainMenu::Awake(pugi::xml_node& node)
{
	LOG("Loading Main menu");

	return true;
}

bool MainMenu::Start()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	bern = app->tex->Load("Assets/Textures/bern.png");
	app->audio->PlayMusic("Assets/Audio/Music/menu_song.ogg", 0.0f);

	app->render->SetBackgroundColor({ 255,255,255,255 });

	change = false;

	gameButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "New Game", { 192,76 }, this);
	playButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Continue", { 192,108 }, this);
	optionsButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Settings", { 192,140 }, this);
	quitButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Quit", { 192,172 }, this);

	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(app->savePath.c_str());
	if(!doc.child("save").child("dungeon").child("current") || !doc.child("save").child("dungeon").child("enemies"))
		playButton->Deactivate();

	return true;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{
	if(!app->options->active)
	{
		if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
			app->input->quit = true;

		if(app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		{
			if(!change)
				app->toLoad = true;
			change = true;
		}
	}

	app->render->SetTextureEvent(2, bern, { 0,0 });

	return true;
}

bool MainMenu::PostUpdate()
{
	return true;
}

bool MainMenu::CleanUp()
{
	app->tex->UnLoad(bern);

	if (!active)
		return true;
	app->ui->DeleteElement(gameButton);
	app->ui->DeleteElement(playButton);
	app->ui->DeleteElement(optionsButton);
	app->ui->DeleteElement(quitButton);

	return true;
}

void MainMenu::UIEvent(Element* element, ElementData&)
{
	if(element == (Element*)playButton)
	{
		if(!change)
			app->toLoad = true;
		change = true;
	}
	else if(element == (Element*)quitButton)
		app->input->quit = true;
	else if(element == (Element*)optionsButton)
		app->options->Activate();
	if (element == (Element*)gameButton)
	{
		if (!change)
		{
			pugi::xml_document doc;
			pugi::xml_parse_result result = doc.load_file(app->savePath.c_str());
			doc.remove_child("save");
			doc.save_file(app->savePath.c_str());

			app->toLoad = true;
		}
		change = true;
	}
}
