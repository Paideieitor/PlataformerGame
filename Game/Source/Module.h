#ifndef __MODULE_H__
#define __MODULE_H__

#include "Defs.h"

#include "pugixml.hpp"

#include <iostream>
#include <string>

using namespace std;

class App;
class Element;

struct ElementData;

class Module
{
public:

	Module() : active(false)
	{}

	void Init()
	{
		active = true;
	}

	void Exit()
	{
		active = false;
		CleanUp();
	}

	// Called before render is available
	virtual bool Awake(pugi::xml_node&)
	{
		return true;
	}

	// Called before the first frame
	virtual bool Start()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PreUpdate()
	{
		return true;
	}

	// Called each loop iteration
	virtual bool Update(float dt)
	{
		return true;
	}

	// Called each loop iteration
	virtual bool PostUpdate()
	{
		return true;
	}

	// Called to save data on the save file
	virtual bool Save(pugi::xml_node&)
	{
		return true;
	}

	// Called to load data from the save file
	virtual bool Load(pugi::xml_node&)
	{
		return true;
	}

	// Called before quitting
	virtual bool CleanUp()
	{
		return true;
	}

	virtual void UIEvent(Element*, ElementData&) {}

	//The returned value needs to be freed from memory
	char* GetMinAndSecFromSec(float time)
	{
		int minutes = time / 60;
		int seconds = time - 60 * minutes;

		char min[10];
		if (minutes / 10 < 1)
			sprintf_s(min, 10, "0%d", minutes);
		else
			sprintf_s(min, 10, "%d", minutes);

		char sec[10];
		if (seconds / 10 < 1)
			sprintf_s(sec, 10, "0%d", seconds);
		else
			sprintf_s(sec, 10, "%d", seconds);

		char* text = new char[10];
		sprintf_s(text, 10, "%s:%s", min, sec);
		return text;
	}

	//The returned value needs to be freed from memory
	char* GetMinAndSecFromSec(uint milisec)
	{
		float time = milisec / 1000.0f;
		int minutes = time / 60;
		int seconds = time - 60 * minutes;

		char min[10];
		if (minutes / 10 < 1)
			sprintf_s(min, 10, "0%d", minutes);
		else
			sprintf_s(min, 10, "%d", minutes);

		char sec[10];
		if (seconds / 10 < 1)
			sprintf_s(sec, 10, "0%d", seconds);
		else
			sprintf_s(sec, 10, "%d", seconds);

		char* text = new char[10];
		sprintf_s(text, 10, "%s:%s", min, sec);
		return text;
	}

public:

	string name;
	bool active;

};

#endif // __MODULE_H__