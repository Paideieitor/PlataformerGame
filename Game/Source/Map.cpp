#include "App.h"
#include "Textures.h"
#include "Render.h"
#include "Collisions.h"
#include "Checkpoints.h"
#include "Map.h"

#include "Log.h"

#include <vector>

#define TILESETS_PATH "Assets/maps/"

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
		for(int i = 0; i < mapData->tSize; i++)
			app->tex->UnLoad(mapData->tilesets[i].texture);
		delete[] mapData->tilesets;

		for(int i = 0; i < mapData->lSize; i++)
		{
			for(int x = 0; x != mapData->layers[i].width; x++)
				delete[] mapData->layers[i].tiles[x];
			delete[] mapData->layers[i].tiles;
		}
		delete[] mapData->layers;

		delete mapData;
		mapData = nullptr;
	}

	return true;
}

WayPoints* Map::LoadMap(char* path)
{
	WayPoints* output = nullptr;

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

	LOG("Loading Map Data---");
	LOG("orientation: %s", mapData->orientation.c_str());
	LOG("render order: %s", mapData->renderOrder.c_str());
	LOG("width: %d", mapData->width);
	LOG("heigth: %d", mapData->height);
	LOG("tile width: %d", mapData->tileWidth);
	LOG("tile height: %d", mapData->tileHeight);

	LoadTilesets(mNode);

	LoadLayers(mNode);

	for(pugi::xml_node node = mNode.child("objectgroup"); node != NULL; node = node.next_sibling("objectgroup"))
	{
		string name = node.attribute("name").as_string();
		if (name == "Colliders")
			LoadColliders(node);
		else if (name == "CheckPoints")
			output = LoadCheckPoints(node);
	}
	
	return output;
}

void Map::DrawMap()
{
	for(int l = 0; l < mapData->lSize; l++)
	{
		start = { 0,0 };
		end = { 0,0 };
		CameraCull(&mapData->layers[l], start.x, start.y, end.x, end.y);

		for(int y = start.y; y != end.y; y++)
		{
			for(int x = start.x; x != end.x; x++)
			{
				uint gid = mapData->layers[l].tiles[x][y];
				Tileset* tileset = GetTileset(gid);
				if(!tileset)
					continue;

				uint id = gid - tileset->firstgid;
				int ny = id / tileset->columns;
				int nx = id - ny * tileset->columns;

				SDL_Rect rect;
				rect.w = tileset->tileWidth;
				rect.h = tileset->tileHeight;
				rect.x = rect.w * nx + tileset->margin * (nx+1);
				rect.y = rect.h * ny + tileset->spacing * (ny+1);

				fPoint position;
				position.x = x * mapData->tileWidth;
				position.y = y * mapData->tileHeight;

				app->render->SetTextureEvent(0, tileset->texture, position, rect);
			}
		}
	}
}

Tileset* Map::GetTileset(uint gid)
{
	for(int i = 0; i < mapData->tSize; i++)
	{
		if(i + 1 == mapData->tSize)
			return &mapData->tilesets[i];
		else
			if (gid < mapData->tilesets[i + 1].firstgid)
				return &mapData->tilesets[i];
			else
				continue;
	}

	return nullptr;
}

void Map::LoadTilesets(pugi::xml_node& mNode)
{
	vector<Tileset> tBuffer;
	for(pugi::xml_node node = mNode.child("tileset"); node != NULL; node = node.next_sibling("tileset"))
	{
		Tileset tileset;

		LOG("%s", node.name());

		tileset.firstgid = node.attribute("firstgid").as_uint();
		tileset.name = node.attribute("name").as_string();
		tileset.tileWidth = node.attribute("tilewidth").as_uint();
		tileset.tileHeight = node.attribute("tileheight").as_uint();
		tileset.margin = node.attribute("margin").as_uint();
		tileset.spacing = node.attribute("spacing").as_uint();
		tileset.count = node.attribute("tilecount").as_uint();
		tileset.columns = node.attribute("columns").as_uint();
		string path = TILESETS_PATH;
		tileset.texture = app->tex->Load((path += node.child("image").attribute("source").as_string()).c_str());

		LOG("Loading Tileset Data---");
		LOG("name: %s", tileset.name.c_str());
		LOG("path: %s", path.c_str());
		LOG("first gid: %d", tileset.firstgid);
		LOG("count: %d", tileset.count);
		LOG("columns: %d", tileset.columns);
		LOG("tile width: %d", tileset.tileWidth);
		LOG("tile height: %d", tileset.tileHeight);
		LOG("spacing: %d", tileset.spacing);
		LOG("margin: %d", tileset.margin);

		tBuffer.push_back(tileset);
	}
	mapData->tSize = tBuffer.size();
	mapData->tilesets = new Tileset[mapData->tSize];
	for(int i = 0; i < mapData->tSize; i++)
		mapData->tilesets[i] = tBuffer[i];
}

void Map::LoadLayers(pugi::xml_node& mNode)
{
	vector<Layer> lBuffer;
	for(pugi::xml_node node = mNode.child("layer"); node != NULL; node = node.next_sibling("layer"))
	{
		Layer layer;

		layer.name = node.attribute("name").as_string();
		layer.width = node.attribute("width").as_uint();
		layer.height = node.attribute("height").as_uint();
		layer.toDraw = node.child("properties").child("property").attribute("value").as_bool();

		LOG("Loading Layer Data---");
		LOG("name: %s", layer.name.c_str());
		LOG("width: %d", layer.width);
		LOG("height: %d", layer.height);
		LOG("toDraw: %d", layer.toDraw);

		layer.tiles = new uint * [layer.width];
		for(int i = 0; i < layer.width; ++i)
			layer.tiles[i] = new uint[layer.height];

		pugi::xml_node lNode = node.child("data").child("tile");
		for(int y = 0; y != layer.height; y++)
		{
			for(int x = 0; x != layer.width; x++)
			{
				int a = lNode.attribute("gid").as_uint();
				layer.tiles[x][y] = a;
				lNode = lNode.next_sibling("tile");
			}
		}

		lBuffer.push_back(layer);
	}
	mapData->lSize = lBuffer.size();
	mapData->layers = new Layer[mapData->lSize];
	for(int i = 0; i < mapData->lSize; i++)
		mapData->layers[i] = lBuffer[i];
}

void Map::LoadColliders(pugi::xml_node& node)
{
	for(pugi::xml_node oNode = node.child("object"); oNode != NULL; oNode = oNode.next_sibling("object"))
	{
		ColliderType type = ColliderType::GROUND;
		string collType = oNode.attribute("type").as_string();
		if(collType == "GROUND")
			type = ColliderType::GROUND;
		if(collType == "ATTACK")
			type = ColliderType::ATTACK;

		int x = oNode.attribute("x").as_int();
		int y = oNode.attribute("y").as_int();
		int w = oNode.attribute("width").as_int();
		int h = oNode.attribute("height").as_int();
		SDL_Rect rect = { x,y,w,h };

		app->collisions->CreateCollider(type, rect);
	}
}

WayPoints* Map::LoadCheckPoints(pugi::xml_node& node)
{
	int size = node.child("properties").child("property").attribute("value").as_int();
	WayPoints* wPoints = new WayPoints(size);

	LOG("Loading CheckPoints ---");
	LOG("current: %d", wPoints->GetCurrent());
	for(pugi::xml_node oNode = node.child("object"); oNode != NULL; oNode = oNode.next_sibling("object"))
	{
		CheckType type = (CheckType)oNode.attribute("type").as_int();
		int x = oNode.attribute("x").as_int();
		int y = oNode.attribute("y").as_int();

		LOG("type: %d | x: %d | y: %d", type, x, y);

		wPoints->AddCheckPoint({ x,y }, type);
	}
	LOG("---");

	return wPoints;
}

void Map::CameraCull(Layer* layer, int& startX, int& startY, int& endX, int& endY)
{
	startX = (-app->render->camera.x) / (int)mapData->tileWidth;
	if(startX < 0)
		startX = 0;
	else if(startX > layer->width - 1)
		startX = layer->width - 1;

	startY = (-app->render->camera.y) / (int)mapData->tileHeight;
	if(startY < 0)
		startY = 0;
	else if(startY > layer->height - 1)
		startY = layer->height - 1;

	endX = (-app->render->camera.x + app->render->camera.w + (int)mapData->tileWidth) / (int)mapData->tileWidth;
	if(endX < 0)
		endX = 0;
	else if(endX > layer->width - 1)
		endX = layer->width - 1;

	endY = (-app->render->camera.y + app->render->camera.h + (int)mapData->tileHeight) / (int)mapData->tileHeight;
	if(endY < 0)
		endY = 0;
	else if(endY > layer->height - 1)
		endY = layer->height - 1;
}
