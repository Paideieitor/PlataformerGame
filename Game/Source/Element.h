#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "Point.h"

class Module;
class Entity;
class Element;

class Animation;

struct SDL_Texture;

enum class ElemType;

struct ElementData
{
	char* name = nullptr;
	int current = 0;
	bool state = false;
};

class Observer
{
public:

	Observer(Module*);
	Observer(Entity*);
	Observer(Element*);

	~Observer();

	void Callback(Element*, ElementData);
	
private:

	enum Type
	{
		NONE = 0,
		MODULE,
		ENTITY,
		ELEMENT
	};

	Type type;

	Module* module = nullptr;
	Entity* entity = nullptr;
	Element* element = nullptr;
};

class Element
{
public:

	enum State
	{
		NORMAL,
		FOCUSED,
		PRESSED,
		DISABLED
	};

	Element(const char* name, ElemType type, fPoint position, iPoint size, Observer* observer, int renderLayer);

	virtual ~Element();

	virtual bool Update(float dt, bool clickable) = 0;

	virtual void UIEvent(Element*, ElementData&);

	void Activate();
	void Deactivate();

	void SetPosition(fPoint);
	fPoint GetPosition();
	fPoint GetDrawPosition(iPoint* size = nullptr);
	void SetSize(iPoint);
	iPoint GetSize();
	ElemType GetType();

	Element* GetElement();

	bool toDelete = false;

protected:

	const char* name;
	bool active = true;

	fPoint position = { 0.0f,0.0f };
	iPoint size = { 0,0 };
	iPoint labelSize = { 0,0 };

	int layer = 0;

	ElemType type = (ElemType)0;
	State state = NORMAL;
	Observer* observer = nullptr;

	Animation* currentAnimation = nullptr;
	Animation* normal = nullptr;
	Animation* focused = nullptr;
	Animation* pressed = nullptr;
	Animation* disabled = nullptr;

	SDL_Texture* texture = nullptr;
	SDL_Texture* label = nullptr;
};

#endif //__ELEMENT_H__