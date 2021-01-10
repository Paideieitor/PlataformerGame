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

WinScene::WinScene() : Module(), newRecord(false)
{
	name = "winscene";
}

WinScene::~WinScene()
{
	delete[] highResults;
	delete[] highTextures;
}

bool WinScene::Awake(pugi::xml_node& node)
{
	LOG("Loading Main menu");

	highTotal = node.attribute("highestScore").as_uint();
	highSize = node.attribute("size").as_uint();

	if(highTotal == 0 || highSize == 0)
		return true;

	highResults = new uint[highSize];
	highTextures = new SDL_Texture * [highSize];

	pugi::xml_node sNode = node.child("dungeon");
	for (uint i = 0; i < highSize; i++)
	{
		highResults[i] = sNode.attribute("score").as_uint();
		sNode = sNode.next_sibling("dungeon");
	}

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

	yourScore = app->tex->Load(app->textFont, "Your Score", ySSize);
	higestScore = app->tex->Load(app->textFont, "Highest Score", hSSize);
	record = app->tex->Load(app->textFont, "NEW RECORD!!!", rSize);

	int timerTotal = 0;
	for(uint i = 0; i < timerSize; i++)
		timerTotal += timerResults[i];
	if(!highResults)
	{
		SetHighScore(timerTotal);

		newRecord = true;
	}
	else
	{
		if(timerTotal < highTotal)
		{
			delete[] highResults;
			delete[] highTextures;

			SetHighScore(highTotal);

			newRecord = true;
		}
	}

	return true;
}

bool WinScene::PreUpdate()
{
	return true;
}

bool WinScene::Update(float dt)
{
	if(app->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		app->input->quit = true;

	if(app->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
		app->fade->ChangeScene(this, app->mainmenu);

	uint total = 0;
	float finalY = 0;

	app->render->SetTextureEvent(40, yourScore, { 0,0 }, { 0, 0, ySSize.x, ySSize.y }, false, false);
	for(int i = 0; i < timerSize; i++)
	{
		if(timerTextures[i])
			app->tex->UnLoad(timerTextures[i]);

		total += timerResults[i];
		char* timer = GetMinAndSecFromSec(timerResults[i]);
		char str[64];
		sprintf_s(str, 64, "Dungeon %d -> %s", i, timer);
		delete timer;

		iPoint textSize;
		timerTextures[i] = app->tex->Load(app->textFont, str, textSize);

		float y = i * textSize.y + 1 + ySSize.y;
		app->render->SetTextureEvent(40, timerTextures[i], { 0,y }, { 0, 0, textSize.x, textSize.y }, false, false);
		finalY = (i + 1) * textSize.y + 1 + ySSize.y;
	}

	if(totalTexture)
		app->tex->UnLoad(totalTexture);

	char* timer = GetMinAndSecFromSec(total);
	char str[64];
	sprintf_s(str, 64, "Total -> %s", timer);
	delete timer;

	iPoint textSize;
	totalTexture = app->tex->Load(app->textFont, str, textSize);
	app->render->SetTextureEvent(40, totalTexture, { 0,finalY }, { 0, 0, textSize.x, textSize.y }, false, false);

	total = 0;
	app->render->SetTextureEvent(40, higestScore, { (float)ySSize.x + 70,0 }, { 0, 0, hSSize.x, hSSize.y }, false, false);
	for (int i = 0; i < highSize; i++)
	{
		if(highTextures[i])
			app->tex->UnLoad(highTextures[i]);

		total += highResults[i];
		char* timer = GetMinAndSecFromSec(highResults[i]);
		char str[64];
		sprintf_s(str, 64, "Dungeon %d -> %s", i, timer);
		delete timer;

		highTextures[i] = app->tex->Load(app->textFont, str, textSize);

		float y = i * textSize.y + 1 + ySSize.y;
		app->render->SetTextureEvent(40, highTextures[i], { (float)ySSize.x + 70,y }, { 0, 0, textSize.x, textSize.y }, false, false);
		finalY = (i + 1) * textSize.y + 1 + ySSize.y;
	}

	if(totalHighTexture)
		app->tex->UnLoad(totalHighTexture);

	timer = GetMinAndSecFromSec(total);
	str[64];
	sprintf_s(str, 64, "Total -> %s", timer);
	delete timer;

	totalHighTexture = app->tex->Load(app->textFont, str, textSize);
	app->render->SetTextureEvent(40, totalHighTexture, { (float)ySSize.x + 70,finalY }, { 0, 0, textSize.x, textSize.y }, false, false);

	if(newRecord)
		app->render->SetTextureEvent(40, record, { 0 ,finalY + 20 }, { 0, 0, rSize.x, rSize.y }, false, false);

	app->render->SetTextureEvent(2, win, { 0,0 });

	return true;
}

bool WinScene::PostUpdate()
{
	return true;
}

bool WinScene::Save(pugi::xml_node& node)
{
	return true;
}

bool WinScene::Load(pugi::xml_node&)
{
	return true;
}

bool WinScene::CleanUp()
{
	newRecord = false;

	app->tex->UnLoad(win);
	if(totalTexture)
		app->tex->UnLoad(totalTexture);
	if(yourScore)
		app->tex->UnLoad(yourScore);
	if(higestScore)
		app->tex->UnLoad(higestScore);
	if(record)
		app->tex->UnLoad(record);

	if(!active)
		return true;
	app->ui->DeleteElement(mainMenuButton);
	app->ui->DeleteElement(quitButton);

	delete[] timerResults;
	timerResults = nullptr;
	for(int i = 0; i < timerSize; i++)
		if(timerTextures[i])
			app->tex->UnLoad(timerTextures[i]);
	delete[] timerTextures;

	for (int i = 0; i < highSize; i++)
		if (highTextures[i])
		{
			app->tex->UnLoad(highTextures[i]);
			highTextures[i] = nullptr;
		}

	return true;
}

void WinScene::UIEvent(Element* element, ElementData&)
{
	if(element == (Element*)mainMenuButton)
		app->fade->ChangeScene(this, app->mainmenu);
	else if(element == (Element*)quitButton)
		app->input->quit = true;
}

void WinScene::SendTimerResults(uint* results, uint size)
{
	if(!timerResults)
	{
		timerSize = size;

		timerResults = new uint[timerSize];
		memcpy(timerResults, results, sizeof(uint) * timerSize);
		timerTextures = new SDL_Texture * [timerSize];
	}
}

void WinScene::SetHighScore(int total)
{
	highSize = timerSize;
	highTotal = total;

	highResults = new uint[highSize];
	memcpy(highResults, timerResults, sizeof(uint) * highSize);
	highTextures = new SDL_Texture * [highSize];

	pugi::xml_node node;
	pugi::xml_document* doc = app->GetConfig(node);

	node.remove_child(name.c_str());
	node = node.append_child(name.c_str());
	node.append_attribute("highestScore").set_value(total);
	node.append_attribute("size").set_value(highSize);

	for(uint i = 0; i < highSize; i++)
		node.append_child("dungeon").append_attribute("score").set_value(highResults[i]);

	doc->save_file(app->configPath.c_str());
}
