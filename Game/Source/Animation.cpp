#include "Animation.h"

#include "SDL.h"

#include <iostream>

Animation::Animation(int size, bool loop, float speed)
{
	this->size = size;
	this->loop = loop;
	this->speed = speed;

	counter = 0;
	current = 0;
	last = 0;

	frames = new SDL_Rect[size];
	memset(frames, 0, sizeof(SDL_Rect) * size);
}

Animation::~Animation()
{
	delete[] frames;
}

bool Animation::PushBack(int x, int y, int w, int h)
{
	if (last < size)
	{
		frames[last] = { x,y,w,h };
		last++;

		return true;
	}

	return false;
}

SDL_Rect Animation::GetFrame(float dt)
{
	counter += dt;
	if(counter >= speed)
	{
		current++;
		if(current == size)
			if(loop)
				current = 0;
			else
				current = size - 1;
		counter = 0;
	}

	return frames[current];
}
