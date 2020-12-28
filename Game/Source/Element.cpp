#include "Element.h"

#include "App.h"
#include "Textures.h"
#include "UIManager.h"

#include "Module.h"
#include "Entity.h"

Observer::Observer(Module* module) : module(module), type(Observer::MODULE)
{
	if(!module)
		type = Observer::NONE;
}

Observer::Observer(Entity* entity) : entity(entity), type(Observer::ENTITY)
{
	if (!entity)
		type = Observer::NONE;
}

Observer::Observer(Element* element) : element(element), type(Observer::ELEMENT)
{
	if (!element)
		type = Observer::NONE;
}

Observer::~Observer()
{
}

void Observer::Callback(Element* element, ElementData data)
{
	switch(type)
	{
	case MODULE:
		module->UIEvent(element, data);
		break;
	case ENTITY:
		entity->UIEvent(element, data);
		break;
	case ELEMENT:
		this->element->UIEvent(element, data);
		break;
	}
}

Element::Element(const char* name, ElemType type, fPoint position, iPoint size, Observer* observer, int renderLayer)
	: name(name), type(type), position(position), size(size), observer(observer), texture(app->ui->uiTexture), layer(renderLayer)
{
}

Element::~Element()
{
	if(observer)
		delete observer;
}

void Element::UIEvent(Element* element, ElementData&)
{
}

void Element::Activate()
{
	if(!active)
	{
		state = State::NORMAL;
		active = true;
	}
}

void Element::Deactivate()
{
	if(active)
	{
		state = State::DISABLED;
		active = false;
	}
}

void Element::SetPosition(fPoint newPosition) { position = newPosition; }

fPoint Element::GetPosition() { return position; }

void Element::SetSize(iPoint newSize) { size = newSize; }

iPoint Element::GetSize() { return size; }

ElemType Element::GetType() { return type; }

Element* Element::GetElement() { return (Element*)this; }

fPoint Element::GetDrawPosition(iPoint* size)
{ 
	if(!size)
		return { position.x - this->size.x / 2, position.y - this->size.y / 2 };
	else
		return { position.x - size->x / 2, position.y - size->y / 2 };
}
