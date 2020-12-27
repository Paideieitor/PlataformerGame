#include "App.h"
#include "Render.h"
#include "Fonts.h"
#include "Textures.h"

#include "Defs.h"
#include "Log.h"

#include "SDL_image.h"

Textures::Textures() : Module()
{
	name = "textures";
}

Textures::~Textures()
{}

bool Textures::Awake(pugi::xml_node& config)
{
	LOG("Init Image library");
	bool ret = true;

	// Load support for the PNG image format
	int flags = IMG_INIT_PNG;
	int init = IMG_Init(flags);

	if((init & flags) != flags)
	{
		LOG("Could not initialize Image lib. IMG_Init: %s", IMG_GetError());
		ret = false;
	}

	return ret;
}

bool Textures::CleanUp()
{
	LOG("Freeing textures and Image library");

	while(textures.size() != 0)
	{
		SDL_DestroyTexture(*textures.begin());
		textures.erase(textures.begin());
	}

	IMG_Quit();
	return true;
}

// Load new texture from file path
SDL_Texture* const Textures::Load(const char* path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path);

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", path, IMG_GetError());
	}
	else
	{
		texture = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	return texture;
}

SDL_Texture* const Textures::Load(Font* font, const char* text, iPoint& size)
{
	SDL_Texture* output = nullptr;

	SDL_Surface* surface = TTF_RenderText_Solid(font->font, text, {0,0,0});

	if(surface == NULL)
	{
		LOG("Could not load surface with path: %s. IMG_Load: %s", TTF_GetError());
	}
	else
	{
		output = LoadSurface(surface);
		SDL_FreeSurface(surface);
	}

	SDL_QueryTexture(output, NULL, NULL, &size.x, &size.y);

	return output;
}

// Unload texture
bool Textures::UnLoad(SDL_Texture* texture)
{
	SDL_Texture* pTexture = NULL;

	for(vector<SDL_Texture*>::iterator t = textures.begin(); t != textures.end(); t++)
	{
		pTexture = *t;

		if(pTexture == texture)
		{
			SDL_DestroyTexture(pTexture);
			textures.erase(t);

			return true;
		}
	}

	return false;
}

// Translate a surface into a texture
SDL_Texture* const Textures::LoadSurface(SDL_Surface* surface)
{
	SDL_Texture* texture = SDL_CreateTextureFromSurface(app->render->renderer, surface);

	if(texture == NULL)
	{
		LOG("Unable to create texture from surface! SDL Error: %s\n", SDL_GetError());
	}
	else
	{
		textures.push_back(texture);
	}

	return texture;
}

// Retrieve size of a texture
void Textures::GetSize(const SDL_Texture* texture, uint& width, uint& height) const
{
	SDL_QueryTexture((SDL_Texture*)texture, NULL, NULL, (int*) &width, (int*) &height);
}
