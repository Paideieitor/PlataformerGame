#include "App.h"
#include "Map.h"
#include "Pathfinding.h"

#include <vector>

Path::Path(vector<fPoint> path)
{
	current = 0;

	size = path.size();
	this->path = new fPoint[size];

	for(uint i = 0; i < size; i++)
		this->path[i] = path[i];
}

Path::~Path()
{
	delete[] path;
}

fPoint Path::NextPoint(fPoint position, bool& end)
{
	end = false;

	fPoint destination = { (float)path[current].x, (float)path[current].y };
	if(position == destination)
	{
		current++;
		if(current == size)
		{
			end = true;
			current--;
		}
	}

	destination = { (float)path[current].x, (float)path[current].y };
	return destination;
}

Node::Node()
{
	tile = { 0,0 };
	walkable = false;

	parent = nullptr;

	g = 0;
	h = 0;
}

int Node::GetF()
{
	return g + h;
}

Pathfinding::Pathfinding()
{
	Quit();
}

void Pathfinding::Init(Layer* layer)
{
	idMap = layer->tiles;
	fid = app->map->GetTileset(idMap[0][0])->firstgid;

	width = layer->width;
	height = layer->height;
}

void Pathfinding::Quit()
{
	idMap = nullptr;
	fid = 0;
	
	width = 0;
	height = 0;
}

Path* Pathfinding::PathTo(fPoint position, fPoint destination)
{
	if(!idMap)
		return nullptr;

	iPoint pos = app->map->WorldToTile(position);
	iPoint dest = app->map->WorldToTile(destination);

	Node** nodeMap = new Node*[width];
	for(int i = 0; i < width; ++i)
		nodeMap[i] = new Node[height];

	for(int y = 0; y != height; y++)
	{
		for(int x = 0; x != width; x++)
		{
			int id = idMap[x][y] - fid;

			bool walkable = false;
			if(id == 0 || id == 2)
				walkable = true;

			nodeMap[x][y].walkable = walkable;
			nodeMap[x][y].tile = { x,y };
		}
	}

	Node* start = &nodeMap[pos.x][pos.y];
	Node* end = &nodeMap[dest.x][dest.y];

	vector<Node*> open;
	vector<Node*> closed;

	Node* current = nullptr;
	vector<Node*>::iterator curr;

	open.push_back(start);

	while(open.size() != 0)
	{
		curr = open.begin();
		current = *curr;
		for(vector<Node*>::iterator n = open.begin(); n != open.end(); n++)
		{
			Node* node = *n;
			if(current == nullptr || node->GetF() < current->GetF() || (node->GetF() == current->GetF() && node->h < current->h))
			{
				current = node;
				curr = n;
			}
		}
		open.erase(curr);

		closed.push_back(current);
		curr = closed.end();

		if(current == end)
		{
			Path* output = CreatePath(current->tile, nodeMap, position, destination);

			CleanUp(nodeMap);
			return output;
		}

		vector<Node*> neighbours = GetNeighbours(current->tile, nodeMap);
		for(uint i = 0; i < neighbours.size(); i++)
			if(neighbours[i]->walkable)
			{
				int g = current->g + GetDistance(current->tile, neighbours[i]->tile);
				int h = GetDistance(neighbours[i]->tile, end->tile);

				bool push = true;
				for(uint n = 0; n < closed.size(); n++)
					if(neighbours[i] == closed[n])
						if(g + h < closed[n]->GetF())
							continue;
						else
							push = false;

				for(uint n = 0; n < open.size(); n++)
					if(neighbours[i] == open[n])
						if(g + h < open[n]->GetF())
							continue;
						else
							push = false;

				if (push)
				{
					neighbours[i]->g = g;
					neighbours[i]->h = h;
					neighbours[i]->parent = current;
					open.push_back(neighbours[i]);
				}
			}
	}

	CleanUp(nodeMap);
	return nullptr;
}

Path* Pathfinding::CreatePath(iPoint currentTile, Node** nodeMap, fPoint start, fPoint end)
{
	vector<fPoint> path;
	vector<iPoint> nodes;

	Node* current = &nodeMap[currentTile.x][currentTile.y];
	while (current->parent != nullptr)
	{
		nodes.push_back(current->tile);

		current = current->parent;
	}
	reverse(nodes.begin(), nodes.end());

	for (uint i = 0; i < nodes.size(); i++)
	{
		fPoint pos = app->map->TileToWorld(nodes[i]);
		path.push_back(pos);
	}

	return new Path(path);
}

vector<Node*> Pathfinding::GetNeighbours(iPoint tile, Node** nodeMap)
{
	vector<Node*> output;

	for(int y = -1; y <= 1; y++)
		for(int x = -1; x <= 1; x++)
		{
			if (x == 0 && y == 0)
				continue;
			else if ((tile.x + x) >= 0 && (tile.x + x) < width && (tile.y + y) >= 0 && (tile.y + y) < height)
				output.push_back(&nodeMap[tile.x + x][tile.y + y]);
		}

	return output;
}

int Pathfinding::GetDistance(iPoint A, iPoint B)
{
	float distance;

	int x = abs(A.x - B.x);
	int y = abs(A.y - B.y);

	distance = sqrt(x * x + y * y) * 10;

	return (int)distance;
}

void Pathfinding::CleanUp(Node** nodeMap)
{
	for (int x = 0; x != width; x++)
		delete[] nodeMap[x];
	delete[] nodeMap;
}