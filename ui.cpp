#ifndef UI_CPP
#define UI_CPP

class UI_Base
{
public:

	float x;
	float y;
	float w;
	float h;

	virtual void Update(){};
	virtual void Render(){};
};

//enum MenuShape{}

class Menu : public UI_Base
{
	
};

namespace InventoryMenu
{
	Menu main;
	Rect square;
	bool active = false;
	float cooldown = 0;

	//Render a Square
	void Tick(float _dt);
	bool HandleEvents(WPARAM wParam, bool keyDown);
	void Render();
}

void InventoryMenu::Tick(float _dt)
{
	if(cooldown > 0){cooldown -= 10 * _dt;}
}

bool InventoryMenu::HandleEvents(WPARAM wParam, bool keyDown)
{
	if(wParam == 0x1B)
	{
		if(cooldown <= 0)
		{
			active = !active; 
			cooldown = 5;
			return true;
		}
	}

	return false;
}

void InventoryMenu::Render()
{
	square(100, 100, 440, 280);
	square.Render("BLUE");
}

#endif