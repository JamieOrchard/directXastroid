#ifndef STARS_H
#define STARS_H

class Star
{
public:
	float x = 0;
	float y = 0;

	void Create();
	void Destroy();
};

void Star::Create()
{
	x = std::rand() % 640;
	y = std::rand() % 480;
}

void Star::Destroy()
{
	x = 0;
	y = 0;
}

class SolarSystem
{
public:

	void Create();
	void AssignXY(float _x, float _y);
	void Render(ID2D1HwndRenderTarget* _RenderTarget, ID2D1SolidColorBrush* _colour);
	void Destroy();

	float start_x = 0;
	float start_y = 0;

	Star star_map[100];
};

void SolarSystem::Create()
{	
	for(int i = 0; i < 100; i++){star_map[i].Create();}
}

void SolarSystem::AssignXY(float _x, float _y)
{
	start_x = _x;
	start_y = _y;
	Create();
}

void SolarSystem::Render(ID2D1HwndRenderTarget* _RenderTarget, ID2D1SolidColorBrush* _colour)
{
	//if(!initilized){return;}

	float temp_start_x = start_x + cameraOffsetX;
	float temp_start_y = start_y + cameraOffsetY;
	float temp_end_x = temp_start_x + 1;
	float temp_end_y = temp_start_y + 1;

	//If offscreen do not render
	if(temp_start_x < -640 || temp_start_x > 640)
	{
		if(temp_start_y < -480 || temp_start_y > 480)
		{
			return;
		}
	}

	for(int i = 0; i < 100; i++)
	{
		Line new_line;
		new_line.start = D2D1::Point2F(star_map[i].x + temp_start_x, star_map[i].y + temp_start_y);
		new_line.end = D2D1::Point2F(star_map[i].x + temp_end_x, star_map[i].y + temp_end_y);

		_RenderTarget->DrawLine(new_line.start, new_line.end, _colour, 2);
	}
}

void SolarSystem::Destroy()
{

}

#endif