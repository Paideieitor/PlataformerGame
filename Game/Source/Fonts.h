#ifndef FONTS_H
#define FONTS_H

#include "Module.h"

#include "SDL_ttf.h"

#include <vector>

struct SDL_Texture;

struct Font
{
	Font(char* buffer, TTF_Font* font);
	~Font();

	char* buffer;
	TTF_Font* font;
};

class Fonts : public Module
{
public:

	Fonts();
	~Fonts();

	bool Awake(pugi::xml_node&);

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	Font* LoadFont(const char* path, int size);

private:

	vector<Font*> fonts;
};

#endif