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
	void Render();
	void Destroy();

	float start_x = 0;
	float start_y = 0;

	static void UpdateSolarSystems(std::vector<SolarSystem>* _systemsVector);

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

void SolarSystem::Render()
{
	float temp_start_x = start_x + cameraOffsetX;
	float temp_start_y = start_y + cameraOffsetY;
	float temp_end_x = temp_start_x + 1;
	float temp_end_y = temp_start_y + 1;

	//if offscreen do not render
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
		new_line.Render("GREY", 2);
	}
}

static void UpdateSolarSystems(std::vector<SolarSystem>& _systemsVector)
{
	float temp_x = 0;
	float temp_y = 0;

	if(cameraOffsetX != 0){temp_x = ceil(-cameraOffsetX / SCREEN_WIDTH);}
	if(cameraOffsetY != 0){temp_y = ceil(-cameraOffsetY / SCREEN_HEIGHT);}

	if(_systemsVector.size() < 200)
	{
		for(int y = -1; y <= 1; y++)
		{
			for(int x = -1; x <= 1; x++)
			{
				bool system_found = false;

				for(auto stars: _systemsVector)
				{
					if(stars.start_x == (temp_x  + x) * SCREEN_WIDTH && stars.start_y == (temp_y + y) * SCREEN_HEIGHT)
					{
						system_found = true;
						break;
					}
				}

				if(!system_found)
				{
					SolarSystem new_star_group;
					new_star_group.AssignXY((temp_x + x) * SCREEN_WIDTH, (temp_y + y) * SCREEN_HEIGHT);
					_systemsVector.push_back(new_star_group);
				}
			}
		}
	}

	for(int i = 0; i < _systemsVector.size(); i++){
		if(_systemsVector.at(i).start_x < -cameraOffsetX - 3000 || _systemsVector.at(i).start_x > -cameraOffsetX + 3000){
			_systemsVector.erase(_systemsVector.begin() + i);
			break;
		}
		if(_systemsVector.at(i).start_y < -cameraOffsetY - 3000 || _systemsVector.at(i).start_y > -cameraOffsetX + 3000)
		{
			_systemsVector.erase(_systemsVector.begin() + i);
			break;
		}
	}
}

void SolarSystem::Destroy()
{

}

#endif