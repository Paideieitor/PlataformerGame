#ifndef __ELEMENT_H__
#define __ELEMENT_H__

#include "Point.h"

class Module;
class Entity;
class Element;

struct Observer
{
public:

	Observer(Module*);
	Observer(Entity*);
	Observer(Element*);

	~Observer();

	void* GetObserver();
	
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

	enum Type
	{
		NONE = 0,
		BUTTON,
		SLIDER,
		INPUT_BOX,
		TEXT
	};

	enum State
	{
		NORMAL,
		FOCUSED,
		PRESSED,
		DISABLED
	};

	Element(const char* name, Type type, fPoint position, iPoint size, Observer* observer);

	virtual ~Element();

	virtual bool Update(float dt) = 0;

	void SetPosition(fPoint);
	fPoint GetPosition();
	void SetSize(iPoint);
	iPoint GetSize();
	Type GetType();

protected:

	fPoint GetDrawPosition();

	const char* name;

	fPoint position = { 0.0f,0.0f };
	iPoint size = { 0,0 };

	Type type = NONE;
	State state = NORMAL;
	Observer* observer = nullptr;
};

#endif //__ELEMENT_H__