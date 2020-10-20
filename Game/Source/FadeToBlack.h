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

private:

	float timer;
	bool out;
	bool in;

	Module* sceneOut;
	Module* sceneIn;
};

#endif //__FADETOBLACK_H__