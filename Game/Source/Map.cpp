#include "Map.h"

#include "Log.h"

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
	if(mapData)
	{
		delete mapData;
		mapData = nullptr;
	}

	return true;
}

bool Map::LoadMap(char* path)
{
	pugi::xml_document document;
	pugi::xml_parse_result result = document.load_file(path);

	mapData = new MapData();
	pugi::xml_node mNode = document.child("map");

	mapData->orientation = mNode.attribute("orientation").as_string();
	mapData->renderOrder = mNode.attribute("renderorder").as_string();
	mapData->width = mNode.attribute("width").as_uint();
	mapData->height = mNode.attribute("height").as_uint();
	mapData->tileWidth = mNode.attribute("tilewidth").as_uint();
	mapData->tileHeight = mNode.attribute("tileheight").as_uint();

	LOG("Map Loaded---");
	LOG("orientation: %s", mapData->orientation.c_str());
	LOG("render order: %s", mapData->renderOrder.c_str());
	LOG("width: %d", mapData->width);
	LOG("heigth: %d", mapData->height);
	LOG("tile width: %d", mapData->tileWidth);
	LOG("tile height: %d", mapData->tileHeight);

	return true;
}