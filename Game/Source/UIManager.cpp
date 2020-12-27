#include "App.h"
#include "Input.h"
#include "Render.h"
#include "Textures.h"
#include "FadeToBlack.h"
#include "Fonts.h"
#include "UIManager.h"

#include "Button.h"

UIManager::UIManager()
{
	name = "ui";
}

UIManager::~UIManager()
{
	if(uiTexture)
		app->tex->UnLoad(uiTexture);
}

bool UIManager::Awake(pugi::xml_node&)
{
	return true;
}

bool UIManager::Start()
{
	uiTexture = app->tex->Load("Assets/Textures/ui.png");

	buttonFont = app->fonts->LoadFont("Assets/Fonts/Overpass/regular.ttf", 16);

	return true;
}

bool UIManager::PreUpdate()
{
	return true;
}

bool UIManager::Update(float dt)
{
	int counter = 0;

	SDL_Rect camera = app->render->camera;
	iPoint mousePosition;
	app->input->GetMousePosition(mousePosition.x, mousePosition.y);
	Element* clickableElem = nullptr;

	if(!app->fade->active)
		for(int i = elements.size() - 1; i >= 0; i--)
		{
			fPoint position = elements[i]->GetDrawPosition();
			iPoint size = elements[i]->GetSize();
			if (position.x < camera.x + mousePosition.x && camera.x + mousePosition.x < position.x + size.x &&
				position.y < camera.y + mousePosition.y && camera.y + mousePosition.y < position.y + size.y)
			{
				clickableElem = elements[i];
				break;
			}
		}

	while(true)
	{
		if(counter >= elements.size())
			break;

		vector<Element*>::iterator elem = elements.begin() + counter;
		for(elem; elem != elements.end(); elem++)
		{
			Element* element = *elem;

			if(!element->toDelete)
			{
				bool clickable = false;
				if(element == clickableElem)
					clickable = true;
				element->Update(dt, clickable);

				counter++;
			}
			else
			{
				delete element;
				elements.erase(elem);
				break;
			}
		}
	}

	return true;
}

bool UIManager::PostUpdate()
{
	for(vector<Element*>::iterator elem = buffer.begin(); elem != buffer.end(); elem++)
		elements.push_back(*elem);
	buffer.erase(buffer.begin(), buffer.end());

	return true;
}

bool UIManager::Save(pugi::xml_node&)
{
	return true;
}

bool UIManager::Load(pugi::xml_node&)
{
	return true;
}

bool UIManager::CleanUp()
{
	while(elements.size() != 0)
	{
		delete* elements.begin();
		elements.erase(elements.begin());
	}
	while(buffer.size() != 0)
	{
		delete* buffer.begin();
		buffer.erase(buffer.begin());
	}

	return true;
}

Element* UIManager::CreateElement(ElemType type, const char* name, fPoint position, Element* observer)
{
	Observer* obs = new Observer(observer);
	Element* output = AddElement(type, name, position, obs);

	if(output)
		return output;

	delete obs;
	return nullptr;
}

Element* UIManager::CreateElement(ElemType type, const char* name, fPoint position, Module* observer)
{
	Observer* obs = new Observer(observer);
	Element* output = AddElement(type, name, position, obs);

	if(output)
		return output;

	delete obs;
	return nullptr;
}

Element* UIManager::CreateElement(ElemType type, const char* name, fPoint position, Entity* observer)
{
	Observer* obs = new Observer(observer);
	Element* output = AddElement(type, name, position, obs);

	if (output)
		return output;

	delete obs;
	return nullptr;
}

void UIManager::DeleteElement(Element* element)
{
	element->toDelete = true;
	element = nullptr;
}

Element* UIManager::AddElement(ElemType type, const char* name, fPoint position, Observer* observer)
{
	Element* output = nullptr;

	switch(type)
	{
	case ElemType::BUTTON:
		output = new Button(name, position, { 100, 40 }, observer);
		break;
	case ElemType::SLIDER:
		break;
	case ElemType::INPUT_BOX:
		break;
	case ElemType::TEXT:
		break;
	}
	if (!output)
		return nullptr;

	buffer.push_back(output);

	return output;
}
