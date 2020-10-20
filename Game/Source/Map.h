#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"

struct SDL_Texture;

struct Tileset
{
	string name;

	SDL_Texture* texture;

	uint firstgid;
	uint count;
	uint columns;

	uint tileWidth;
	uint tileHeight;

	uint margin;
	uint spacing;
};

struct Layer
{
	string name;

	uint width;
	uint height;
	bool toDraw;

	uint** tiles;
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
	uint tSize;
	Layer* layers;
	uint lSize;
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
	void DrawMap();

private:

	Tileset* GetTileset(uint gid);

	MapData* mapData;
};
#endif
