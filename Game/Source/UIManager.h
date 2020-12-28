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

struct SDL_Texture;
struct Font;

struct UIData
{
	int renderLayer = 20;
	char** list = nullptr;
	int size = 0;
	int selected = 0;
	bool checked = false;
};

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

	Element* CreateElement(ElemType type, const char* name, fPoint position, Element* observer, UIData data = UIData());
	Element* CreateElement(ElemType type, const char* name, fPoint position, Module* observer, UIData data = UIData());
	Element* CreateElement(ElemType type, const char* name, fPoint position, Entity* observer, UIData data = UIData());

	void ActivateAll();
	void DeactivateAll();

	void DeleteElement(Element* element);

	SDL_Texture* uiTexture = nullptr;
	Font* buttonFont = nullptr;

private:

	Element* AddElement(ElemType type, const char* name, fPoint position, Observer* observer, UIData data);

	vector<Element*> elements;
	vector<Element*> buffer;
};

#endif //__UIMANAGER_H__