#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"

struct MapData
{
	string orientation; 
	string renderOrder;

	uint width;
	uint height;
	uint tileWidth;
	uint tileHeight;

	TileSet* tilesets;
	Layer* layers;
};

struct TileSet
{

};

struct Layer
{

};

class Map : public Module
{
public:

	Map();
	virtual ~Map();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	bool CleanUp();

private:

	MapData* mapData;
};
#endif
