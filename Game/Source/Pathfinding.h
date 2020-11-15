#ifndef __PATHFINDING_H__
#define __PATHFINDING_H__

#include "Point.h"

#include <vector>

struct Layer;

class Path
{
public:

	Path(std::vector<fPoint> path);
	~Path();

	fPoint NextPoint(fPoint position, bool &end);

private:

	fPoint* path;
	int size;
	int current;
};

struct Node
{
	Node();

	int GetF();

	iPoint tile;
	bool walkable;

	Node* parent = nullptr;

	int g;
	int h;
};

class Pathfinding
{
public:

	Pathfinding();

	void Init(Layer*);
	void Quit();

	Path* PathTo(fPoint position, fPoint destination);

	uint** idMap = nullptr;
	uint fid;

	uint width;
	uint height;

private:

	Path* CreatePath(iPoint currentTile, Node** nodeMap, fPoint start, fPoint end);
	vector<Node*> GetNeighbours(iPoint tile, Node** nodeMap);
	void CleanUp(Node** nodeMap);

	int GetDistance(iPoint A, iPoint B);
};

#endif //__PATHFINDING_H__