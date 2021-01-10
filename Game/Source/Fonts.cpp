#include "Fonts.h"

Font::Font(char* buffer, TTF_Font* font)
{
	this->buffer = buffer;
	this->font = font;
}

Font::~Font()
{
	TTF_CloseFont(font);
}

Fonts::Fonts()
{
	name = "fonts";
}

Fonts::~Fonts()
{
}

bool Fonts::Awake(pugi::xml_node&)
{
	return true;
}

bool Fonts::Start()
{
	TTF_Init();

	return true;
}

bool Fonts::Update(float dt)
{
	return true;
}

bool Fonts::CleanUp()
{
	while(fonts.size() != 0)
	{
		delete* fonts.begin();
		fonts.erase(fonts.begin());
	}

	TTF_Quit();

	return true;
}

Font* Fonts::LoadFont(const char* path, int size)
{
	char* buffer = nullptr;
	TTF_Font* font = TTF_OpenFont(path, size);

	if(!font)
	{
		cout << "Loading font -> Bad Thing, Error in " << path << " -> " << SDL_GetError() << endl;
		return nullptr;
	}

	Font* output = new Font(buffer, font);
	fonts.push_back(output);

	return output;
}
