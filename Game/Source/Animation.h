#ifndef __ANIMATION_H__
#define __ANIMATION_H__

struct SDL_Rect;

class Animation
{
public:

	Animation(int size, bool loop = false, float speed = 1.0f);
	~Animation();

	bool PushBack(int x, int y, int w, int h);
	SDL_Rect GetFrame(float dt);

	float speed;
	bool loop;

private:

	SDL_Rect* frames;
	int size;

	int current;
	float counter;
	int last;
};

#endif //__ANIMATION_H__