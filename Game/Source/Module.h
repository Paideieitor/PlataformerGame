#ifndef __MODULE_H__
#define __MODULE_H__

#include "Defs.h"

#include "pugixml.hpp"

#include <iostream>
#include <string>

using namespace std;

class App;
class Element;

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

	virtual void UIEvent(Element*) {}

public:

	string name;
	bool active;

};

#endif // __MODULE_H__