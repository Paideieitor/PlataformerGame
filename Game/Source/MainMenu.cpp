#include "MainMenu.h"

#include "Defs.h"
#include "Log.h"

MainMenu::MainMenu() : Module()
{
	name = "main menu";
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
	return true;
}

bool MainMenu::PreUpdate()
{
	return true;
}

bool MainMenu::Update(float dt)
{
	return true;
}

bool MainMenu::PostUpdate()
{
	return true;
}

bool MainMenu::CleanUp()
{
	return true;
}