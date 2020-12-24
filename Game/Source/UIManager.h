#ifndef __UIMANAGER_H__
#define __UIMANAGER_H__

#include "Module.h"

#include "ElemType.h"
#include "Point.h"

#include <vector>

class Element;
class Module;
class Entity;
class Observer;

class UIManager : public Module
{
public:

	UIManager();
	virtual ~UIManager();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	bool CleanUp();

	Element* CreateElement(ElemType type, const char* name, fPoint position, Element* observer);
	Element* CreateElement(ElemType type, const char* name, fPoint position, Module* observer);
	Element* CreateElement(ElemType type, const char* name, fPoint position, Entity* observer);

	void DeleteElement(Element* element);

private:

	Element* AddElement(ElemType type, const char* name, fPoint position, Observer* observer);

	vector<Element*> elements;
	vector<Element*> buffer;
};

#endif //__UIMANAGER_H__