#include "FadeToBlack.h"

#include "App.h"
#include "Render.h"

FadeToBlack::FadeToBlack()
{
	name = "fadetoblack";

	sceneOut = nullptr;
	sceneIn = nullptr;
	out = false;
	in = false;
	alpha = 0;
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Update(float dt)
{
	if(out)
	{
		alpha++;
		if(alpha >= 255)
		{
			alpha = 255;

			out = false;
			in = true;

			sceneOut->CleanUp();
			sceneOut->active = false; 
			sceneOut = nullptr;

			sceneIn->Start();
			sceneIn->active = true;
			sceneIn = nullptr;
		}
	}
	else if(in)
	{
		alpha--;
		if(alpha <= 0)
		{
			alpha = 0;

			in = false;

			active = false;
		}
	}

	app->render->SetRectangleEvent(10, {0,0}, {app->render->camera.w, app->render->camera.h}, 0, 0, 0, (int)alpha, false);

	return true;
}

void FadeToBlack::ChangeScene(Module* sceneOut, Module* sceneIn)
{
	if (!active)
	{
		active = true;

		out = true;
		this->sceneOut = sceneOut;
		this->sceneIn = sceneIn;
	}
}