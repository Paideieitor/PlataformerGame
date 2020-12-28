#include "App.h"
#include "Audio.h"
#include "Textures.h"
#include "Render.h"
#include "Input.h"
#include "FadeToBlack.h"
#include "MainMenu.h"
#include "WinScene.h"

#include "UIManager.h"
#include "Button.h"

#include "Defs.h"
#include "Log.h"

WinScene::WinScene() : Module()
{
	name = "mainmenu";
}

WinScene::~WinScene()
{}

bool WinScene::Awake(pugi::xml_node& node)
{
	LOG("Loading Main menu");

	return true;
}

bool WinScene::Start()
{
	app->render->camera.x = 0;
	app->render->camera.y = 0;

	win = app->tex->Load("Assets/Textures/bern_win.png");
	app->audio->PlayMusic("Assets/Audio/Music/win_song.ogg", 0.0f);

	mainMenuButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Main Menu", { 192, 102 }, this);
	quitButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Quit", { 192,140 }, this);

	app->render->SetBackgroundColor({ 255,255,255,255 });

	return true;
}

bool WinScene::PreUpdate()
{
	return true;
}

bool WinScene::Update(float dt)
{
	if (app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		app->input->quit = true;

	if(app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->ChangeScene(this, app->mainmenu);

	app->render->SetTextureEvent(2, win, { 0,0 });

	return true;
}

bool WinScene::PostUpdate()
{
	return true;
}

bool WinScene::CleanUp()
{
	app->tex->UnLoad(win);

	if(!active)
		return true;
	app->ui->DeleteElement(mainMenuButton);
	app->ui->DeleteElement(quitButton);

	return true;
}

void WinScene::UIEvent(Element* element, ElementData&)
{
	if(element == (Element*)mainMenuButton)
		app->fade->ChangeScene(this, app->mainmenu);
	else if(element == (Element*)quitButton)
		app->input->quit = true;
}
