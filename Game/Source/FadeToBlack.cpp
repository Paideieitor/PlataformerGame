#include "App.h"
#include "Render.h"
#include "Options.h"
#include "Pause.h"
#include "FadeToBlack.h"

FadeToBlack::FadeToBlack()
{
	name = "fadetoblack";

	sceneOut = nullptr;
	sceneIn = nullptr;
	out = false;
	in = false;
	timer = 0;
}

FadeToBlack::~FadeToBlack()
{

}

bool FadeToBlack::Update(float dt)
{
	if(out)
	{
		timer += dt;
		if(timer >= 1)
		{
			timer = 1;

			out = false;
			in = true;

			sceneOut->CleanUp();
			sceneOut->active = false; 
			sceneOut = nullptr;
			app->options->Deactivate();
			app->pause->Deactivate();

			sceneIn->Start();
			sceneIn->active = true;
			current = sceneIn;
			sceneIn = nullptr;
		}
	}
	else if(in)
	{
		timer -= dt;
		if(timer <= 0)
		{
			timer = 0;

			in = false;

			active = false;
		}
	}

	app->render->SetRectangleEvent(1000, {0,0}, {app->render->camera.w, app->render->camera.h}, 0, 0, 0, timer * 255, false);

	return true;
}

void FadeToBlack::ChangeScene(Module* sceneOut, Module* sceneIn)
{
	if(!active)
	{
		active = true;

		out = true;
		this->sceneOut = sceneOut;
		this->sceneIn = sceneIn;
	}
}