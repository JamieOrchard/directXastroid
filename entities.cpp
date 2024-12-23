#ifndef ENTITIES_H
#define ENTITIES_H

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

class Astroid
{
public:
	static void InitalizePointList();


	void Create();
	void Destroy();

	void Update(float _dt);
	void Render(ID2D1HwndRenderTarget* _RenderTarget);

	void IncreaseX(float _x);
	void IncreaseRot(float _rot);

	void  SetRotation(float _rot);
	float GetRotation();

	float GetX();
	float GetY();

	int GetSize();
	int GetCollisionSize();
	bool CheckCollision(float _x, float _y);

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

void Astroid::Render(ID2D1HwndRenderTarget* _RenderTarget)
{
	std::vector<Line>* astroid_size;
	if(size == 3){astroid_size = &astroid_size_3;}
	if(size == 2){astroid_size = &astroid_size_2;}
	if(size == 1){astroid_size = &astroid_size_1;}
	if(size == 0){return;} //Safety

	for(auto i: *astroid_size)
	{
		Line tempLine;
		tempLine = i;
		D2D_POINT_2F newPoint = {x + cameraOffsetX, y + cameraOffsetY};
		tempLine.AdjustProjection(newPoint);
		int centerPoint = 4;
		centerPoint = centerPoint << size; //Bit Shift based on size
		tempLine.CenterTo(centerPoint);
		tempLine.RotateProjection(x + cameraOffsetX, y + cameraOffsetY, rotation);
		_RenderTarget->DrawLine(tempLine.start, tempLine.end, COLOURS::palette["CYAN"], 2);
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
			return 35;
		case 2:
			return 20;
		case 1:
			return 10;
		default:
			return 0;
	}
}

bool Astroid::CheckCollision(float _x, float _y)
{
	int a = _x - x;
	int b = _y - y;
	int c = sqrt(a*a + b*b);

	if(c < GetCollisionSize()){
		Hit();
		return true;
	}

	return false;
}

void UpdateAstroidList(float dt)
{
	for(int i = 0; i < astroid_list.size(); i++)
	{
		astroid_list.at(i).Update(dt);
	}
}

std::vector<Line> playerLines;

class Bullet
{
public:
	float x = 0;
	float y = 0;

	float vecX = 0;
	float vecY = 0;
	float speed = 400;

	float MAX_LIFESPAN = 5;
	float current_life = 0;

	void Create(float _x, float _y, float _vecX, float _vecY);
	void Update(float _dt);
	void Render(ID2D1HwndRenderTarget* _RenderTarget);
	bool CheckAlive();
	void Destroy();
};

void Bullet::Create(float _x, float _y, float _vecX, float _vecY)
{
	x = _x - cameraOffsetX;
	y = _y - cameraOffsetY;
	vecX = _vecX;
	vecY = _vecY;
}

void Bullet::Update(float _dt)
{
	x += (vecX * speed) * _dt;
	y += (vecY * speed) * _dt;
	current_life = current_life + _dt;	
}

void Bullet::Render(ID2D1HwndRenderTarget* _RenderTarget)
{
	Line new_line;
	new_line.start = D2D1::Point2F(x + cameraOffsetX, y + cameraOffsetY);
	new_line.end = D2D1::Point2F(x + cameraOffsetX + (10 * vecX) , y + cameraOffsetY + (10 * vecY));
	_RenderTarget->DrawLine(new_line.start, new_line.end, COLOURS::palette["RED"], 2);
}

bool Bullet::CheckAlive()
{
	if(MAX_LIFESPAN > current_life){return true;}
	return false;
}

class Player
{
public:
	//static void InitalizePointList();
	
	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool fire = false;

	float start_x = 640/2;
	float start_y = 480/2;
	float x = 0;
	float y = 0;
	float rotation = 0;

	float vecX = 0;
	float vecY = 0;
	float VECMOD = 10;
	float VECMAX = 200;
	float ROTSPEED = 2;

	D2D_RECT_F movement_box;

	std::vector<Bullet> player_bullets;
	int PLAYER_BULLETS_MAX = 20;
	
	void Create();
	bool HandleInput(WPARAM, bool);
	void Update(float _dt);
	void Render(ID2D1HwndRenderTarget*);
};

/*
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
*/

void Player::Create()
{
	x = 640 / 2;
	y = 480 / 2;

	movement_box.left = 310;
	movement_box.top = 230;
	movement_box.right = 330;
	movement_box.bottom = 250;
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
	if(wParam == 0x20)
	{
		if(!keyDown){fire = true;}
	}


	return false;
}

void Player::Update(float _dt)
{
	//Check input and clamp rotations
	if(left){rotation -= ROTSPEED * _dt;}
	if(right){rotation += ROTSPEED * _dt;}

	//Find the projected vector if the player is moving in that direction
	float unitVecX = cos(rotation - 1.57);
	float unitVecY = sin(rotation - 1.57);

	//Apply projected vector if key is pressed
	if(up){
		vecX -= unitVecX * VECMOD;
		vecY -= unitVecY * VECMOD;

		if(vecX >  VECMAX * unitVecX){vecX =  VECMAX * unitVecX;}
		if(vecX < -VECMAX * unitVecX){vecX = -VECMAX * unitVecX;}
		if(vecY >  VECMAX * unitVecY){vecY =  VECMAX * unitVecY;}
		if(vecY < -VECMAX * unitVecY){vecY = -VECMAX * unitVecY;}
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

	//Apply movement vector to posistion
	cameraOffsetX += vecX * _dt;
	cameraOffsetY += vecY * _dt;

	//Bullet Handling
	if(fire)
	{
		fire = false;
		if(player_bullets.size() < PLAYER_BULLETS_MAX){
			Bullet new_bullet;
			new_bullet.Create(start_x, start_y, unitVecX, unitVecY);
			player_bullets.push_back(new_bullet);
		}
	}

	if(!player_bullets.empty()){
		for(int i = 0; i < player_bullets.size(); i++){
			if(!player_bullets.at(i).CheckAlive())
			{
				player_bullets.erase(player_bullets.begin() + i);
				break;
			}
		}
	}

	int count = 0;
	for(auto& bullet: player_bullets)
	{
		bullet.Update(_dt);
		count++;
	}
}

void Player::Render(ID2D1HwndRenderTarget* _RenderTarget)
{
	for(auto i: GeometricShapes::player)
	{
		Line tempLine;
		tempLine = i;
		D2D_POINT_2F newPoint = {start_x, start_y};
		tempLine.AdjustProjection(newPoint);
		int centerPoint = 16;
		tempLine.CenterTo(centerPoint);
		tempLine.RotateProjection(start_x, start_y, rotation);
		_RenderTarget->DrawLine(tempLine.start, tempLine.end, COLOURS::palette["WHITE"], 2);
	}

	//_RenderTarget->DrawRectangle(&movement_box, COLOURS::palette["GREY"]);

	for(auto& i: player_bullets)
	{
		i.Render(_RenderTarget);
	}
}

#endif