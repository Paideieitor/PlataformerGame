#ifndef __FADETOBLACK_H__
#define __FADETOBLACK_H__

#include "Module.h"

class FadeToBlack : public Module
{
public:

	FadeToBlack();
	virtual ~FadeToBlack();

	bool Update(float dt);

	void ChangeScene(Module* sceneOut, Module* sceneIn);

	Module* current = nullptr;

private:

	float timer;
	bool out;
	bool in;

	Module* sceneOut = nullptr;
	Module* sceneIn = nullptr;
};

#endif //__FADETOBLACK_H__