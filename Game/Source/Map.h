#ifndef __MAP_H__
#define __MAP_H__

#include "Module.h"

#include "Point.h"

struct SDL_Texture;
class WayPoints;

struct Tileset
{
	string name;

	SDL_Texture* texture = nullptr;

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

	uint** tiles = nullptr;
};

struct MapData
{
	string orientation;
	string renderOrder;

	uint width;
	uint height;
	uint tileWidth;
	uint tileHeight;

	Tileset* tilesets = nullptr;
	uint tSize;
	Layer* layers = nullptr;
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

	WayPoints* LoadMap(char* path);
	void DrawMap();

	iPoint WorldToTile(fPoint position);
	fPoint TileToWorld(iPoint tile);

	Tileset* GetTileset(uint gid);

private:

	void LoadTilesets(pugi::xml_node&);
	void LoadLayers(pugi::xml_node&);
	void LoadColliders(pugi::xml_node&);
	WayPoints* LoadCheckPoints(pugi::xml_node&);

	void CameraCull(Layer* layer, int& startX, int& startY, int& endX, int& endY);

	MapData* mapData = nullptr;

	iPoint start;
	iPoint end;
};
#endif
