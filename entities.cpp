#ifndef ENTITIES_H
#define ENTITIES_H

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

float cameraOffsetX = 0;
float cameraOffsetY = 0;

class Line
{
public:
	D2D_POINT_2F start;
	D2D_POINT_2F end;

	Line();
	~Line();

	Line& operator=(const Line& other); //Copy Constructor
	
	void CenterTo(float pos);
	void AdjustProjection(D2D_POINT_2F adjustment);
	void RotateProjection(float x, float y, float rotation);
};

Line::Line()
{
	start = {0, 0};
	end = {0,0};
}

Line::~Line()
{
	start = {0, 0};
	end = {0, 0};
}

Line& Line::operator=(const Line& other)
{	
	start = other.start;
	end = other.end;
	return *this;
}

void Line::CenterTo(float pos)
{
	start.x -= pos;
	start.y -= pos;
	end.x -= pos;
	end.y -= pos;
}

void Line::AdjustProjection(D2D_POINT_2F adjustment)
{
	start.x += adjustment.x;
	start.y += adjustment.y;
	end.x += adjustment.x;
	end.y += adjustment.y;
}

void Line::RotateProjection(float x, float y, float rotation)
{
	float s = sin(rotation);
	float c = cos(rotation);

	//translate point back to origin
	start.x -= x;
	start.y -= y;
	end.x -= x;
	end.y -= y;

	//rotate point
	float xstartnew = start.x * c - start.y * s;
	float ystartnew = start.x * s + start.y * c;
	float xendnew = end.x * c - end.y * s;
	float yendnew = end.x * s + end.y * c;

	//translate points back
	start.x = xstartnew + x;
	start.y = ystartnew + y;
	end.x = xendnew + x;
	end.y = yendnew + y;
}

class Star
{
public:
	float x = 0;
	float y = 0;

	void Create();
};

void Star::Create()
{
	x = std::rand() % 640;
	y = std::rand() % 480;
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
	for(int i = 0; i < 100; i++)
	{
		bool overlap = false;

		do
		{
			star_map[i].Create();
			for(int j = 0; j < 100; j++)
			{
				if(star_map[i].x == star_map[j].x && star_map[i].y == star_map[j].y && i != j){overlap = true;}
			}
		}while(overlap);	
	}
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

	for(int i = 0; i < 100; i++)
	{
		Line new_line;
		new_line.start = D2D1::Point2F(star_map[i].x + temp_start_x, star_map[i].y + temp_start_y);
		new_line.end = D2D1::Point2F(star_map[i].x + temp_end_x, star_map[i].y + temp_end_y);

		_RenderTarget->DrawLine(new_line.start, new_line.end, _colour, 2);
	}
}

SolarSystem newSystem;

class Astroid
{
public:
	static void InitalizePointList();


	void Create();
	void Destroy();

	void Update(float _dt);
	void Render(ID2D1HwndRenderTarget* _RenderTarget, ID2D1SolidColorBrush* _colour);

	void IncreaseX(float _x);
	void IncreaseRot(float _rot);

	void  SetRotation(float _rot);
	float GetRotation();

	float GetX();
	float GetY();

	int GetSize();
	int GetCollisionSize();

	bool GetImmune();

	void Hit();
	

private:
	float x   = 0.f;
	float y   = 0.f;
	float rotation = 0.f;
	float rotationSpeed = 0.f;
	float vecX = 0.f;
	float vecY = 0.f;
	int size  = 3;
	bool immune = true;
	float immunity_timer = 3;
	float immunity_max_timer = 1;
};

std::vector<Line> astroid_size_3;
std::vector<Line> astroid_size_2;
std::vector<Line> astroid_size_1;
std::vector<Line> astroidLineList;
std::vector<Astroid> astroid_list;
int astroid_collision_size[] = {5, 10, 30};

void Astroid::InitalizePointList()
{
	//Astroid Size 3
	Line new_line;
	new_line.start = D2D1::Point2F(31, 4);
	new_line.end = D2D1::Point2F(60, 20);
	astroid_size_3.push_back(new_line);
	new_line.start = D2D1::Point2F(60,21);
	new_line.end = D2D1::Point2F(56,60);
	astroid_size_3.push_back(new_line);
	new_line.start = D2D1::Point2F(56,61);
	new_line.end = D2D1::Point2F(5,56);
	astroid_size_3.push_back(new_line);
	new_line.start = D2D1::Point2F(5,55);
	new_line.end = D2D1::Point2F(8,28);
	astroid_size_3.push_back(new_line);
	new_line.start = D2D1::Point2F(8,27);
	new_line.end = D2D1::Point2F(6,17);
	astroid_size_3.push_back(new_line);
	new_line.start = D2D1::Point2F(6,16);
	new_line.end = D2D1::Point2F(31,4);
	astroid_size_3.push_back(new_line);

	//Astroid Size 2
	// 15,4 ; 27,13 ; 27,27 ; 3, 29; 7,4
	new_line.start = D2D1::Point2F(15,4);
	new_line.end = D2D1::Point2F(27,13);
	astroid_size_2.push_back(new_line);
	new_line.start = D2D1::Point2F(27,13);
	new_line.end = D2D1::Point2F(27,27);
	astroid_size_2.push_back(new_line);
	new_line.start = D2D1::Point2F(27,27);
	new_line.end = D2D1::Point2F(3,29);
	astroid_size_2.push_back(new_line);
	new_line.start = D2D1::Point2F(3,29);
	new_line.end = D2D1::Point2F(7,4);
	astroid_size_2.push_back(new_line);
	new_line.start = D2D1::Point2F(7,4);
	new_line.end = D2D1::Point2F(15,4);
	astroid_size_2.push_back(new_line);

	//Astroid Size 1
	// 9,1; 13,6; 10,14; 2,11; 3,7
	new_line.start = D2D1::Point2F(9,1);
	new_line.end = D2D1::Point2F(13,6);
	astroid_size_1.push_back(new_line);
	new_line.start = D2D1::Point2F(13,6);
	new_line.end = D2D1::Point2F(10,14);
	astroid_size_1.push_back(new_line);
	new_line.start = D2D1::Point2F(10,14);
	new_line.end = D2D1::Point2F(2,11);
	astroid_size_1.push_back(new_line);
	new_line.start = D2D1::Point2F(2,11);
	new_line.end = D2D1::Point2F(3,7);
	astroid_size_1.push_back(new_line);
	new_line.start = D2D1::Point2F(3,7);
	new_line.end = D2D1::Point2F(9,1);
	astroid_size_1.push_back(new_line);
}

void Astroid::Create()
{
	int direction = std::rand() % 4;
	rotation = std::rand() % 180;
	rotationSpeed = (std::rand() % 100 + 1) / 10;
	//Spawn above
	if(direction == 0){
		y = -32;
		x = std::rand() % 640;
		vecX = cos(rotation);
		vecY = sin(rotation); 
	}
	//Spawn Right
	if(direction == 1){
		y = std::rand() % 480;
		x = 640;
		vecX = -cos(rotation);
		vecY =  sin(rotation);
	}
	//Spawn Down
	if(direction == 2){
		y = 600;
		x = std::rand() % 640;
		vecX = cos(rotation);
		vecY = -sin(rotation);
	} 
	//Spawn Left
	if(direction == 3){
		y = std::rand() % 480;
		x = -100;
		vecX = cos(rotation);
		vecY = sin(rotation);
	}

	x += cameraOffsetX;
	y += cameraOffsetY;
}

void Astroid::Update(float _dt)
{
	x += vecX * (_dt * 50);
	y += vecY * (_dt * 50);
	rotation += rotationSpeed * _dt;

	if(vecX > 0 && x + cameraOffsetX > 700){Create();}
	if(vecX < 0 && x + cameraOffsetX < -100){Create();}
	if(vecY > 0 && y + cameraOffsetY > 500){Create();}
	if(vecY < 0 && y + cameraOffsetY < -100){Create();}

	if(immune && immunity_timer < 0){immune = false;}
	else{immunity_timer = immunity_timer - _dt;}
}

void Astroid::Render(ID2D1HwndRenderTarget* _RenderTarget, ID2D1SolidColorBrush* _colour)
{
	for(auto astroid: astroid_list)
	{
		std::vector<Line>* astroid_size;
		if(astroid.size == 3){astroid_size = &astroid_size_3;}
		if(astroid.size == 2){astroid_size = &astroid_size_2;}
		if(astroid.size == 1){astroid_size = &astroid_size_1;}
		if(astroid.size == 0){continue;} //Safety

		for(auto i: *astroid_size)
		{
			Line tempLine;
			tempLine = i;
			D2D_POINT_2F newPoint = {astroid.x + cameraOffsetX, astroid.y + cameraOffsetY};
			tempLine.AdjustProjection(newPoint);
			int centerPoint = 4;
			centerPoint = centerPoint << astroid.size; //Bit Shift based on size
			tempLine.CenterTo(centerPoint);
			tempLine.RotateProjection(astroid.x + cameraOffsetX, astroid.y + cameraOffsetY, astroid.rotation);
			_RenderTarget->DrawLine(tempLine.start, tempLine.end, _colour, 2);
		}
	}
}	


void Astroid::Hit()
{
	if(size == 1){return;}
	Astroid firstAstroid;
	Astroid secondAstroid;
	float rotational_change = 0.5;

	firstAstroid.Create();
	secondAstroid.Create();

	firstAstroid.x = x;
	firstAstroid.y = y;
	firstAstroid.immune = true;
	firstAstroid.immunity_timer = firstAstroid.immunity_max_timer;
	firstAstroid.size = size - 1;
	firstAstroid.SetRotation(rotation - 10);
	firstAstroid.vecX = cos(rotational_change) * vecX - sin(rotational_change) * vecY;
	firstAstroid.vecY = sin(rotational_change) * vecX + cos(rotational_change) * vecY;

	//Look at rotating the movement vector by so they fly in the same direction as parent but also away from each child

	secondAstroid.x = x;
	secondAstroid.y = y;
	secondAstroid.immune = true;
	secondAstroid.immunity_timer = secondAstroid.immunity_max_timer;
	secondAstroid.size = size - 1;
	secondAstroid.SetRotation(rotation + 10);
	secondAstroid.vecX = cos(-rotational_change) * vecX - sin(-rotational_change) * vecY;
	secondAstroid.vecY = sin(-rotational_change) * vecX + cos(-rotational_change) * vecY;

	astroid_list.push_back(firstAstroid);
	astroid_list.push_back(secondAstroid);
}

void Astroid::IncreaseX(float _x){x += _x;}
void Astroid::IncreaseRot(float _rot){rotation += _rot;}

float Astroid::GetRotation(){return rotation;}
void Astroid::SetRotation(float _rot){rotation = _rot;}

float Astroid::GetX(){return x;}
float Astroid::GetY(){return y;}

bool Astroid::GetImmune(){return immune;}
int Astroid::GetSize(){return size;}
int Astroid::GetCollisionSize()
{
	switch(size)
	{
		case 3:
			return 30;
		case 2:
			return 15;
		case 1:
			return 5;
		default:
			return 0;
	}
}

void UpdateAstroidList(float dt)
{
	for(int i = 0; i < astroid_list.size(); i++)
	{
		astroid_list.at(i).Update(dt);
	}
}

std::vector<Line> playerLines;

class Player
{
public:
	static void InitalizePointList();
	
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;

	float start_x = 250;
	float start_y = 250;
	float x = 0;
	float y = 0;
	float rotation = 0;

	float vecX = 0;
	float vecY = 0;
	float VECMOD = 1;
	float VECMAX = 100;
	float ROTSPEED = 2;
	
	void Create();
	bool HandleInput(WPARAM, bool);
	void Update(float _dt);
	void Render(ID2D1HwndRenderTarget*, ID2D1SolidColorBrush*);

};

void Player::InitalizePointList()
{
	Line new_line;
	new_line.start = D2D1::Point2F(16, 0);
	new_line.end = D2D1::Point2F(31, 31);
	playerLines.push_back(new_line);
	new_line.start = D2D1::Point2F(31, 31);
	new_line.end = D2D1::Point2F(0, 31);
	playerLines.push_back(new_line);
	new_line.start = D2D1::Point2F(0, 31);
	new_line.end = D2D1::Point2F(15, 0);
	playerLines.push_back(new_line);
}

void Player::Create()
{
	x = 250;
	y = 250;
}

bool Player::HandleInput(WPARAM wParam, bool keyDown)
{
	if(wParam == 0x57)
	{
		if(!up &&  keyDown){up = true;}
		if( up && !keyDown){up = false;}
	}
	if(wParam == 0x53)
	{
		if(!down &&  keyDown){down = true;}
		if( down && !keyDown){down = false;}
	}
		if(wParam == 0x41)
	{
		if(!left &&  keyDown){left = true;}
		if( left && !keyDown){left = false;}
	}
	if(wParam == 0x44)
	{
		if(!right &&  keyDown){right = true;}
		if( right && !keyDown){right = false;}
	}


	return false;
}

void Player::Update(float _dt)
{
	//Declare some setup variables once
	float VECMODDT = VECMOD * _dt;

	//Check input and clamp rotations
	if(left){rotation -= ROTSPEED * _dt;}
	if(right){rotation += ROTSPEED * _dt;}

	//Change readable degrees to Radians
	//float radians = rotation * (M_PI / 180);

	//Find the projected vector if the player is moving in that direction
	float unitVecX = cos(rotation - 1.57);
	float unitVecY = sin(rotation - 1.57);

	//Apply projected vector if key is pressed
	if(up){
		vecX -= unitVecX;
		vecY -= unitVecY;

		//Clamp movement vector based on rotation have the ABS to deal with negative values inversions)
		/*
		float vecMaxX = abs(unitVecX * VECMAX);
		float vecMaxY = abs(unitVecY * VECMAX);
		if(vecX >  vecMaxX){vecX = -vecMaxX;}
		if(vecX < -vecMaxX){vecX =  vecMaxX;}
		if(vecY >  vecMaxY){vecY = -vecMaxY;}
		if(vecY < -vecMaxY){vecY =  vecMaxY;}
		*/
	}
	if(!up){
		if(vecX < VECMOD && vecX > -VECMOD){vecX = 0;}
		else{
			if(vecX > 0){vecX -= VECMOD;}
			if(vecX < 0){vecX += VECMOD;}
		}
		if(vecY < VECMOD && vecY > -VECMOD){vecY = 0;}
		else{
			if(vecY > 0){vecY -= VECMOD;}
			if(vecY < 0){vecY += VECMOD;}
		}
	}

	//Apple movement vector to posistion
	x += vecX * _dt;
	y += vecY * _dt;

	cameraOffsetX = x;
	cameraOffsetY = y;
}

void Player::Render(ID2D1HwndRenderTarget* _RenderTarget, ID2D1SolidColorBrush* _colour)
{
	for(auto i: playerLines)
	{
		Line tempLine;
		tempLine = i;
		D2D_POINT_2F newPoint = {start_x, start_y};
		tempLine.AdjustProjection(newPoint);
		int centerPoint = 16;
		tempLine.CenterTo(centerPoint);
		tempLine.RotateProjection(start_x, start_y, rotation);
		_RenderTarget->DrawLine(tempLine.start, tempLine.end, _colour, 2);
	}
}

namespace Game
{
	Player player;
	std::vector<SolarSystem> star_systems;

	void Init();
	void Update(float _delta);
	void Render();
}

void Game::Init()
{
	Astroid::InitalizePointList();
	Player::InitalizePointList();
	player.Create();

	/*
	float temp_x = -640;
	float temp_y = -480;

	for(int y = 0; y < 3; y++)
	{
		temp_x = -640;
		for(int x = 0; x < 3; x++)
		{
			SolarSystem new_star_group;
			new_star_group.AssignXY(temp_x, temp_y);
			star_systems.push_back(new_star_group);
			temp_x += 640;
		}
		temp_y += 480;
	}
	*/
}

void Game::Update(float _delta)
{

	player.Update(_delta);

	//Update rendering of stars, generate new and remove old.
	//Get the X camera offset divide by 640 then times it by the ceil version should give the start_x closest to the camera
	
	float temp_x = 0;
	float temp_y = 0;

	float screen_width = 640;
	float screen_height = 480;

	/*
	if(cameraOffsetX != 0){
		if(Game::player.vecX < 0 ){temp_x = floor(cameraOffsetX / 640) * -640;}
		if(Game::player.vecX > 0 ){temp_x = ceil(cameraOffsetX / 640) * -640;}
	}
	if(cameraOffsetY != 0){
		if(Game::player.vecY < 0 ){temp_y = floor(cameraOffsetY / 480) * -480;}
		if(Game::player.vecY > 0 ){temp_y = ceil(cameraOffsetY / 480) * -480;}
	}
	*/

	if(cameraOffsetX != 0){temp_x = ceil(cameraOffsetX / 640);}
	if(cameraOffsetY != 0){temp_y = ceil(cameraOffsetY / 640);}

	if(star_systems.size() < 20)
	{
		for(int y = -1; y <= 1; y++)
		{
			for(int x = -1; x <= 1; x++)
			{
				bool system_found = false;

					for(auto stars: star_systems)
					{
						if(stars.start_x == (temp_x  + x) * -640 && stars.start_y == (temp_y + y) * -480)
						{
							system_found = true;
							break;
						}
					}

					if(!system_found)
					{
						SolarSystem new_star_group;
						new_star_group.AssignXY((temp_x + x) * -640, (temp_y + y) * -480);
						star_systems.push_back(new_star_group);
						printf("New Star Generated at X: %f Y: %f X2: %d Y2: %d\n", (temp_x + x) * -640, (temp_y + y) * -480, x, y);
					}

			}
		}
	}
	else
	{
		int count = 0;
		for(auto stars: star_systems)
		{
			printf("Star_System At [%d] X: %f Y: %f\n", count, stars.start_x, stars.start_y);
			count++;
		}
	}

	for(int i = 0; i < star_systems.size(); i++){
		if(star_systems.at(i).start_x < Game::player.x - 2000){
			//star_systems.erase()
		}
	}

}

#endif