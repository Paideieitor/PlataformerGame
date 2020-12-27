#ifndef __TEXTURES_H__
#define __TEXTURES_H__

#include "Module.h"

#include "Point.h"

struct SDL_Texture;
struct SDL_Surface;
struct Font;

class Textures : public Module
{
public:

	Textures();
	virtual ~Textures();

	bool Awake(pugi::xml_node&);

	bool CleanUp();

	// Load Texture
	SDL_Texture* const Load(const char* path);
	SDL_Texture* const Load(Font* font, const char* text, iPoint& size);
	SDL_Texture* const LoadSurface(SDL_Surface* surface);
	bool UnLoad(SDL_Texture* texture);
	void GetSize(const SDL_Texture* texture, uint& width, uint& height) const;

public:

	vector<SDL_Texture*> textures;
};


#endif // __TEXTURES_H__