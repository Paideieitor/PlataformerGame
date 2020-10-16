#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"

struct SDL_Texture;

struct Tileset
{
	string name;

	SDL_Texture* texture;

	uint firstgid;
	uint width;
	uint height;
};

struct Tile
{
	uint id;

	Tileset* tileset;
	//layer->layerdata = new int* [layer_width];
	//for (int i = 0; i < layer_width; ++i)
	//	layer->layerdata[i] = new int[layer_height];
};

struct Layer
{
	string name;

	uint width;
	uint height;

	Tile** tiles;
};

struct MapData
{
	string orientation;
	string renderOrder;

	uint width;
	uint height;
	uint tileWidth;
	uint tileHeight;

	Tileset* tilesets;
	Layer* layers;
};

class Map : public Module
{
public:

	Map();
	virtual ~Map();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	bool CleanUp();

	bool LoadMap(char* path);

private:

	MapData* mapData;
};
#endif
