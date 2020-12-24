#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "Point.h"

class Module;
class Entity;
class Element;

class Animation;

enum class ElemType;

class Observer
{
public:

	Observer(Module*);
	Observer(Entity*);
	Observer(Element*);

	~Observer();

	void Callback(Element*);
	
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

	Element(const char* name, ElemType type, fPoint position, iPoint size, Observer* observer);

	virtual ~Element();

	virtual bool Update(float dt, bool clickable) = 0;

	virtual void UIEvent(Element*);

	void SetPosition(fPoint);
	fPoint GetPosition();
	void SetSize(iPoint);
	iPoint GetSize();
	ElemType GetType();

	Element* GetElement();

	bool toDelete = false;

protected:

	fPoint GetDrawPosition();

	const char* name;

	fPoint position = { 0.0f,0.0f };
	iPoint size = { 0,0 };

	ElemType type = (ElemType)0;
	State state = NORMAL;
	Observer* observer = nullptr;

	Animation* currentAnimation = nullptr;
	Animation* normal = nullptr;
	Animation* focused = nullptr;
	Animation* pressed = nullptr;
	Animation* disabled = nullptr;
};

#endif //__ELEMENT_H__