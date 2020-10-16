#include "Map.h"

Map::Map()
{
	name = "map";
}

Map::~Map()
{

}

bool Map::Awake(pugi::xml_node&)
{
	return true;
}

bool Map::Start()
{
	return true;
}

bool Map::PreUpdate()
{
	return true;
}

bool Map::Update(float dt)
{
	return true;
}

bool Map::PostUpdate()
{
	return true;
}

bool Map::Save(pugi::xml_node&)
{
	return true;
}

bool Map::Load(pugi::xml_node&)
{
	return true;
}

bool Map::CleanUp()
{
	return true;
}