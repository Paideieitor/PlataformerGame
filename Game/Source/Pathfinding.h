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
	void DrawPath();

private:

	fPoint* path;
	int size;
	int current;

	friend fPoint* GetPathInfo(const Path &path, int& size, int &current);
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

	static Pathfinding* GetInstance();
	~Pathfinding();

	void Init(Layer*);
	void Quit();

	Path* PathTo(fPoint position, fPoint destination, bool(*decoder)(int), bool walkMode = false);
	
	uint** idMap = nullptr;
	uint fid;

	uint width;
	uint height;

private:

	Pathfinding();
	Pathfinding(const Pathfinding&);
	Pathfinding& operator=(const Pathfinding&);

	static Pathfinding* instance;

	Path* CreatePath(iPoint currentTile, Node** nodeMap, fPoint start, fPoint end);
	vector<Node*> GetNeighbours(iPoint tile, Node** nodeMap);
	void CleanUp(Node** nodeMap);

	int GetDistance(iPoint A, iPoint B);
};


#endif //__PATHFINDING_H__