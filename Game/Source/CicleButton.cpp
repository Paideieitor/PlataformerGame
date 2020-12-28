#include "CicleButton.h"

#include "App.h"
#include "Textures.h"
#include "Render.h"

#include "UIManager.h"
#include "Button.h"

CicleButton::CicleButton(const char* name, char** list, int size, int selected, fPoint position, Observer* observer, int renderLayer)
	: Element(name, ElemType::CICLE_BUTTON, position, { 0,0 }, observer, renderLayer), list(list), size(size), current(selected)
{
	label = app->tex->Load(app->ui->buttonFont, name, labelSize);

	cicleButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, list[current], { position.x - 55, position.y + 22 }, this, { renderLayer });
	applyButton = (Button*)app->ui->CreateElement(ElemType::BUTTON, "Apply", { position.x + 55, position.y + 22}, this, { renderLayer });
}

CicleButton::~CicleButton()
{
	app->tex->UnLoad(label);

	app->ui->DeleteElement(cicleButton);
	app->ui->DeleteElement(applyButton);

	delete list;
}

bool CicleButton::Update(float dt, bool clickable)
{
	app->render->SetTextureEvent(layer, label, GetDrawPosition(&labelSize), { 0,0,labelSize.x, labelSize.y }, false, false);

	return true;
}

void CicleButton::UIEvent(Element* element, ElementData&)
{
	if(element == (Element*)cicleButton)
	{
		current++;
		if(current >= size)
			current = 0;

		cicleButton->Rename(list[current]);
	}
	else if(element == (Element*)applyButton)
		observer->Callback(this, { list[current], current });
}
