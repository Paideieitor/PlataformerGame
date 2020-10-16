#include "App.h"
#include "Window.h"
#include "Render.h"

#include "Defs.h"
#include "Log.h"

#define VSYNC true

Render::Render() : Module()
{
	name = "render";
	background.r = 100;
	background.g = 50;
	background.b = 70;
	background.a = 0;
}

Render::~Render()
{}

bool Render::Awake(pugi::xml_node& config)
{
	LOG("Create SDL rendering context");

	Uint32 flags = SDL_RENDERER_ACCELERATED;

	if(config.child("vsync").attribute("value").as_bool(true) == true)
	{
		flags |= SDL_RENDERER_PRESENTVSYNC;
		LOG("Using vsync");
	}

	renderer = SDL_CreateRenderer(app->win->window, -1, flags);

	if(renderer == NULL)
	{
		LOG("Could not create the renderer! SDL_Error: %s\n", SDL_GetError());
		return false;
	}
	else
	{
		camera.w = app->win->screenSurface->w;
		camera.h = app->win->screenSurface->h;
		camera.x = 0;
		camera.y = 0;
	}

	return true;
}

bool Render::Start()
{
	LOG("render start");
	// back background
	SDL_RenderGetViewport(renderer, &viewport);
	return true;
}

bool Render::PreUpdate()
{
	SDL_RenderClear(renderer);
	return true;
}

bool Render::Update(float dt)
{
	for(map<int, DrawEvent>::iterator m = drawEvents.begin(); m != drawEvents.end(); m++)
	{
		DrawEvent event = m->second;
		switch(event.type)
		{
		case DrawType::TEXTURE:
			DrawTexture(event.texture, event.position.x, event.position.y, &event.rect, event.useCamera, event.speed, event.angle, event.pivotX, event.pivotY);
			break;
		case DrawType::RECTANGLE:
			DrawRectangle(event.rect, event.color.r, event.color.g, event.color.b, event.color.a, event.useCamera, event.filled);
			break;
		case DrawType::LINE:
			DrawLine(event.rect.x, event.rect.y, event.rect.w, event.rect.h, event.color.r, event.color.g, event.color.b, event.color.a, event.useCamera);
			break;
		case DrawType::CIRCLE:
			DrawCircle(event.rect.x, event.rect.y, event.rect.w, event.color.r, event.color.g, event.color.b, event.color.a, event.useCamera);
			break;
		}
	}

	multimap<int, DrawEvent> trash;
	trash.swap(drawEvents);

	return true;
}

bool Render::PostUpdate()
{
	SDL_SetRenderDrawColor(renderer, background.r, background.g, background.g, background.a);
	SDL_RenderPresent(renderer);
	return true;
}

bool Render::CleanUp()
{
	LOG("Destroying SDL render");
	SDL_DestroyRenderer(renderer);
	return true;
}

bool Render::Save(pugi::xml_node& node)
{
	pugi::xml_node mNode = node.child("camera");
	if(mNode == NULL)
	{
		mNode = node.append_child("camera");
		mNode.append_attribute("x");
		mNode.append_attribute("y");
	}

	mNode.attribute("x").set_value(camera.x);
	mNode.attribute("y").set_value(camera.y);

	return true;
}

bool Render::Load(pugi::xml_node& node)
{
	pugi::xml_node mNode = node.child("camera");
	if(mNode == NULL)
	{
		LOG("Camera Loading Error -> no camera node found");
		return false;
	}

	camera.x = mNode.attribute("x").as_int();
	camera.y = mNode.attribute("y").as_int();

	return true;
}

void Render::SetBackgroundColor(SDL_Color color)
{
	background = color;
}

void Render::SetTextureEvent(int layer, SDL_Texture* texture, fPoint position, SDL_Rect section, bool useCamera, float speed, double angle, int pivotX, int pivotY)
{
	DrawEvent event;

	event.type = DrawType::TEXTURE;
	event.texture = texture;
	event.position = position;
	event.rect = section;
	event.useCamera = useCamera;
	event.speed = speed;
	event.angle = angle;
	event.pivotX = pivotX;
	event.pivotY = pivotY;

	drawEvents.insert(std::make_pair(layer, event));
}
void Render::SetRectangleEvent(int layer, fPoint position, iPoint size, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera, bool filled)
{
	DrawEvent event;

	event.type = DrawType::RECTANGLE;
	event.rect = {(int)position.x, (int)position.y, size.x, size.y};
	event.color = { r,g,b,a };
	event.useCamera = useCamera;
	event.filled = filled;

	drawEvents.insert(std::make_pair(layer, event));
}
void Render::SetLineEvent(int layer, int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera) 
{
	DrawEvent event;

	event.type = DrawType::LINE;
	event.rect = { x1,y1,x2,y2 };
	event.color = { r,g,b,a };
	event.useCamera = useCamera;

	drawEvents.insert(std::make_pair(layer, event));
}

void Render::SetCircleEvent(int layer, int x1, int y1, int redius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool useCamera)
{
	DrawEvent event;

	event.type = DrawType::LINE;
	event.rect = { x1,y1,redius,0 };
	event.color = { r,g,b,a };
	event.useCamera = useCamera;

	drawEvents.insert(std::make_pair(layer, event));
}

void Render::SetViewPort(const SDL_Rect& rect)
{
	SDL_RenderSetViewport(renderer, &rect);
}

void Render::ResetViewPort()
{
	SDL_RenderSetViewport(renderer, &viewport);
}

// Blit to screen
bool Render::DrawTexture(SDL_Texture* texture, int x, int y, const SDL_Rect* section, bool useCamera, float speed, double angle, int pivotX, int pivotY) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	if(section->w == 0 || section->h == 0)
		section = nullptr;

	SDL_Rect rect;
	if(useCamera)
	{
		rect.x = (int)(camera.x * speed) + x * scale;
		rect.y = (int)(camera.y * speed) + y * scale;
	}
	else
	{
		rect.x = x;
		rect.y = y;
	}

	if(section != NULL)
	{
		rect.w = section->w;
		rect.h = section->h;
	}
	else
	{
		SDL_QueryTexture(texture, NULL, NULL, &rect.w, &rect.h);
	}

	if(useCamera)
	{
		rect.w *= scale;
		rect.h *= scale;
	}

	SDL_Point* p = NULL;
	SDL_Point pivot;

	if(pivotX != INT_MAX && pivotY != INT_MAX)
	{
		pivot.x = pivotX;
		pivot.y = pivotY;
		p = &pivot;
	}

	if(SDL_RenderCopyEx(renderer, texture, section, &rect, angle, p, SDL_FLIP_NONE) != 0)
	{
		LOG("Cannot blit to screen. SDL_RenderCopy error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawRectangle(const SDL_Rect& rect, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera, bool filled) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	SDL_Rect rec(rect);
	if(use_camera)
	{
		rec.x = (int)(camera.x + rect.x * scale);
		rec.y = (int)(camera.y + rect.y * scale);
		rec.w *= scale;
		rec.h *= scale;
	}

	int result = (filled) ? SDL_RenderFillRect(renderer, &rec) : SDL_RenderDrawRect(renderer, &rec);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawLine(int x1, int y1, int x2, int y2, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;

	if(use_camera)
		result = SDL_RenderDrawLine(renderer, camera.x + x1 * scale, camera.y + y1 * scale, camera.x + x2 * scale, camera.y + y2 * scale);
	else
		result = SDL_RenderDrawLine(renderer, x1 * scale, y1 * scale, x2 * scale, y2 * scale);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}

bool Render::DrawCircle(int x, int y, int radius, Uint8 r, Uint8 g, Uint8 b, Uint8 a, bool use_camera) const
{
	bool ret = true;
	uint scale = app->win->GetScale();

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);

	int result = -1;
	SDL_Point points[360];

	float factor = (float)M_PI / 180.0f;

	for(uint i = 0; i < 360; ++i)
	{
		points[i].x = (int)(x + radius * cos(i * factor));
		points[i].y = (int)(y + radius * sin(i * factor));
	}

	result = SDL_RenderDrawPoints(renderer, points, 360);

	if(result != 0)
	{
		LOG("Cannot draw quad to screen. SDL_RenderFillRect error: %s", SDL_GetError());
		ret = false;
	}

	return ret;
}