#ifndef __RENDER_H__
#define __RENDER_H__

#include "Module.h"

#include "Point.h"

#include "SDL.h"

#include <map>

enum class DrawType
{
	TEXTURE,
	RECTANGLE,
	CIRCLE,
	LINE
};

struct DrawEvent
{
	DrawType type;

	SDL_Texture* texture;
	fPoint position;
	SDL_Rect rect;
	SDL_Color color;
	bool useCamera;
	bool filled;
	float speed;
	double angle;
	int pivotX;
	int pivotY;
};

class Render : public Module
{
public:

	Render();
	virtual ~Render();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool Save(pugi::xml_node&);
	bool Load(pugi::xml_node&);

	bool CleanUp();

	void SetViewPort(const SDL_Rect& rect);
	void ResetViewPort();

	// Set background color
	void SetBackgroundColor(SDL_Color color);

	void SetTextureEvent(int layer, SDL_Texture* texture, fPoint position, SDL_Rect section = { 0,0,0,0 }, bool useCamera = true, float speed = 1.0f, int alpha = 255, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX);
	void SetRectangleEvent(int layer, fPoint position, iPoint size, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true, bool filled = true);
	void SetLineEvent(int layer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true);
	void SetCircleEvent(int layer, int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true);

	SDL_Renderer* renderer;
	SDL_Rect camera;
	SDL_Rect viewport;
	SDL_Color background;

private:

	multimap<int, DrawEvent> drawEvents;

	// Drawing
	bool DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section = NULL, bool useCamera = true, float speed = 1.0f, int alpha = 255, double angle = 0, int pivotX = INT_MAX, int pivotY = INT_MAX) const;
	bool DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true, bool filled = true) const;
	bool DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
	bool DrawCircle(int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255, bool useCamera = true) const;
};

#endif // __RENDER_H__