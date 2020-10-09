#include "Module.h"

class MainMenu : public Module
{
public:

	MainMenu();
	virtual ~MainMenu();

	bool Awake(pugi::xml_node&);
	bool Start();

	bool PreUpdate();
	bool Update(float dt);
	bool PostUpdate();

	bool CleanUp();

private:
};