#ifndef __CHECKPOINTS_H__
#define __CHECKPOINTS_H__

#include "Point.h"
#include <iostream>

enum class CheckType
{
	NONE = 0,
	START = 1,
	RESPAWN = 2,
	END = 3
};

struct CheckPoint
{
	iPoint position;
	CheckType type;
};

class WayPoints
{
public:

	WayPoints(int size)
	{
		current = 0;
		last = 0;

		this->size = size;
		checkpoints = new CheckPoint[size];
		memset(checkpoints, 0, sizeof(CheckPoint) * size);
	}
	~WayPoints()
	{
		delete[] checkpoints;
	}

	CheckPoint* checkpoints;
	int size;

	void AddCheckPoint(iPoint position, CheckType type)
	{
		if (last != size)
		{
			checkpoints[last] = { position, type };
			last++;
		}
	}
	void Reset()
	{
		current = 0;
	}

private:

	int current;
	int last;
};

#endif //__CHECKPOINTS_H__
