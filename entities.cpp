#ifndef ENTITIES_H
#define ENTITIES_H

#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

class Ore;

class BaseEntity
{
public:

	float x;
	float y;
	float rotation;
	float vecX;
	float vecY;
	float unitVecX;
	float unitVecY;

	float GetX(){return x;}
	float GetY(){return y;}
	float GetRotation(){return rotation;}
	float GetVecX(){return vecX;}
	float GetVecY(){return vecY;}
};

class Astroid : public BaseEntity
{
public:

	void Create();
	void Destroy();

	void Update(float _dt);
	void Render();

	void IncreaseRot(float _rot){rotation += _rot;}
	void  SetRotation(float _rot){rotation = _rot;}
	int GetSize(){return size;}
	int GetCollisionSize();
	bool CheckCollision(float _x, float _y);

	void Hit(std::vector<Astroid>& _astroidBuffer);
	

private:
	float rotationSpeed = 0.f;
	int size  = 3;

	static const int BORDER_BOUNDARIES = 200;
};

int astroid_collision_size[] = {5, 10, 30};

void Astroid::Create()
{
	int direction = std::rand() % 4;
	rotation = std::rand() % 180;
	rotationSpeed = (std::rand() % 100 + 1) / 10;
	//Spawn above
	if(direction == 0){
		y = -32;
		x = (std::rand() % 640);
		vecX = cos(rotation);
		vecY = sin(rotation); 
	}
	//Spawn Right
	if(direction == 1){
		y = (std::rand() % 480);
		x = 640;
		vecX = -cos(rotation);
		vecY =  sin(rotation);
	}
	//Spawn Down
	if(direction == 2){
		y = 600;
		x = (std::rand() % 640);
		vecX = cos(rotation);
		vecY = -sin(rotation);
	} 
	//Spawn Left
	if(direction == 3){
		y = (std::rand() % 480);
		x = -32;
		vecX = cos(rotation);
		vecY = sin(rotation);
	}

	x -= cameraOffsetX;
	y -= cameraOffsetY;
}

void Astroid::Update(float _dt)
{
	x += vecX * (_dt * (rotationSpeed * 20));
	y += vecY * (_dt * (rotationSpeed * 20));
	rotation += rotationSpeed * _dt;

	if(vecX > 0 && x + cameraOffsetX >  640 + BORDER_BOUNDARIES){Create();}
	if(vecX < 0 && x + cameraOffsetX <    0 - BORDER_BOUNDARIES){Create();}
	if(vecY > 0 && y + cameraOffsetY >  480 + BORDER_BOUNDARIES){Create();}
	if(vecY < 0 && y + cameraOffsetY <    0 - BORDER_BOUNDARIES){Create();}	
}

void Astroid::Render()
{
	std::vector<Line>* astroid_size;
	if(size == 3){astroid_size = &GeometricShapes::astroids_size_3;}
	if(size == 2){astroid_size = &GeometricShapes::astroids_size_2;}
	if(size == 1){astroid_size = &GeometricShapes::astroids_size_1;}
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
		tempLine.Render("CYAN", 2);
		//renderTarget->DrawLine(tempLine.start, tempLine.end, COLOURS::palette["CYAN"], 2);
	}
}	

void Astroid::Hit(std::vector<Astroid>& _astroidBuffer)
{
	if(size == 1){return;}
	Astroid firstAstroid;
	Astroid secondAstroid;
	float rotational_change = 0.5;

	firstAstroid.Create();
	secondAstroid.Create();

	firstAstroid.x = x;
	firstAstroid.y = y;
	firstAstroid.size = size - 1;
	firstAstroid.SetRotation(rotation - 10);
	firstAstroid.vecX = cos(rotational_change) * vecX - sin(rotational_change) * vecY;
	firstAstroid.vecY = sin(rotational_change) * vecX + cos(rotational_change) * vecY;

	//Look at rotating the movement vector by so they fly in the same direction as parent but also away from each child

	secondAstroid.x = x;
	secondAstroid.y = y;
	secondAstroid.size = size - 1;
	secondAstroid.SetRotation(rotation + 10);
	secondAstroid.vecX = cos(-rotational_change) * vecX - sin(-rotational_change) * vecY;
	secondAstroid.vecY = sin(-rotational_change) * vecX + cos(-rotational_change) * vecY;

	_astroidBuffer.push_back(firstAstroid);
	_astroidBuffer.push_back(secondAstroid);
}



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

	if(c < GetCollisionSize()){return true;}

	return false;
}

enum OreType{Copper, Silver, Gold};

class Ore : public BaseEntity
{
public:
	OreType ore_type;
	D2D1_ELLIPSE ellipse;

	void Create(OreType _type, float _x, float _y);
	void CreateOnAstroid(int _size, float _x, float _y);
	void CheckCollision(std::vector<Line>& _points);
	void Update();
	void Render();
	void Destroy();

	float GetX(){return ellipse.point.x;}
	float GetY(){return ellipse.point.y;}

};

//std::vector<Ore> oreBuffer;

void Ore::Create(OreType _type, float _x, float _y)
{
	ore_type = _type;
	x = _x;
	y = _y;
}

void Ore::CreateOnAstroid(int _size, float _x, float _y)
{
	switch(_size)
	{
	case 3:
		ore_type = OreType::Copper;
		ellipse.radiusX = 8;
		ellipse.radiusY = 8;
		break;
	case 2:
		ore_type = OreType::Silver;
		ellipse.radiusX = 4;
		ellipse.radiusY = 4;
		break;
	case 1:
		ore_type = OreType::Gold;
		ellipse.radiusX = 2;
		ellipse.radiusY = 2;
		break;
	default:
		return;
	}

	ellipse.point.x = _x;
	ellipse.point.y = _y;
}

void Ore::Update()
{

}

void Ore::CheckCollision(std::vector<Line>& _points)
{
	//Triangle vs Circle collision

}

void Ore::Render()
{
	std::string colour;
	switch(ore_type)
	{
		case(Copper):
			colour = "ORANGE";
		break;
		case(Silver):
			colour = "GREY";
		break;
		case(Gold):
			colour = "YELLOW";
		break;
	}

	//Add render line here
	D2D1_ELLIPSE temp;
	temp = ellipse;
	temp.point.x = ellipse.point.x + cameraOffsetX;
	temp.point.y = ellipse.point.y + cameraOffsetY;
	renderTarget->DrawEllipse(&temp, COLOURS::palette[colour], 2, NULL);
}

class Bullet : public BaseEntity
{
public:

	float speed = 400;

	float MAX_LIFESPAN = 5;
	float current_life = 0;

	void Create(float _x, float _y, float _vecX, float _vecY);
	void Update(float _dt);
	void Render();
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

void Bullet::Render()
{
	Line new_line;
	new_line.start = D2D1::Point2F(x + cameraOffsetX, y + cameraOffsetY);
	new_line.end = D2D1::Point2F(x + cameraOffsetX + (10 * vecX) , y + cameraOffsetY + (10 * vecY));
	new_line.Render("RED", 2);
}

bool Bullet::CheckAlive()
{
	if(MAX_LIFESPAN > current_life){return true;}
	return false;
}

class Player : public BaseEntity
{
public:
	
	bool alive = true;

	bool up = false;
	bool down = false;
	bool left = false;
	bool right = false;
	bool fire = false;

	float start_x = 640/2;
	float start_y = 480/2;

	float VECMOD = 10;
	float VECMAX = 200;
	float ROTSPEED = 2;

	float centerPoint = 16;

	D2D_RECT_F movement_box;

	std::vector<Bullet> player_bullets;
	int PLAYER_BULLETS_MAX = 20;
	
	void Create();
	bool HandleInput(WPARAM, bool);
	void Update(float _dt);
	void Render();
	void DeadRender();
	void CheckAstroidCollisions(std::vector<Astroid>& _astroidBuffer);

	D2D_POINT_2F GetStartPoint();

	float death_movement_counter = 0.0f;
	float death_movement_increase = 0.0f;

	bool GetAlive(){return alive;}

private:
	void UpdateMovement(float _dt);
	void UpdateBullets(float _dt);
	void UpdateRespawn(float _dt);
	
};

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
	if(alive)
	{
		UpdateMovement(_dt);
		UpdateBullets(_dt);
	}
	else
	{
		UpdateRespawn(_dt);
	}	
}

void Player::UpdateMovement(float _dt)
{
	//Check input and clamp rotations
	if(left){rotation -= ROTSPEED * _dt;}
	if(right){rotation += ROTSPEED * _dt;}

	//Find the projected vector if the player is moving in that direction
	unitVecX = cos(rotation - 1.57);
	unitVecY = sin(rotation - 1.57);

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
}

void Player::UpdateBullets(float _dt)
{
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

void Player::UpdateRespawn(float _dt)
{
	death_movement_counter += ((_dt * 100) * death_movement_increase);

	if(fire){
		fire = false;
		alive = true;
		cameraOffsetX = 0;
		cameraOffsetY = 0;
		rotation = 0;
		vecX = 0;
		vecY = 0;
		death_movement_increase = 0;
		death_movement_counter = 0;

		player_bullets.clear();
	}
}

void Player::CheckAstroidCollisions(std::vector<Astroid>& _astroidBuffer)
{
		if(GeometricShapes::player.size() == 0){return;}
		std::vector<Line> lines = GeometricShapes::player;
		for(auto& line : lines){
			line.AdjustProjection(GetStartPoint());
			line.CenterTo(centerPoint);
			line.RotateProjection(start_x, start_y, rotation);
		}

		for(auto astroid : _astroidBuffer){
			D2D_POINT_2F astroidPoint = D2D1::Point2F(astroid.GetX() + cameraOffsetX, astroid.GetY() + cameraOffsetY);
			D2D_POINT_2F nearestPoint = Collisions::NearestPointOnTriangle(astroidPoint, lines.at(0).GetOffset(), lines.at(1).GetOffset(), lines.at(2).GetOffset());
			if(Collisions::CircleTriangle(nearestPoint, astroidPoint, astroid.GetCollisionSize())){
				alive = false;
				vecX = astroid.vecX;
				vecY = astroid.vecY;
				//Calcuate Unit vector for rotation
				float hyp = sqrt(vecX * vecX + vecY * vecY);
				rotation = atan2(vecY / hyp, vecX / hyp);
				death_movement_increase = hyp;
			}
		}
}

void Player::Render()
{
	for(auto i: GeometricShapes::player)
	{
		Line tempLine;
		tempLine = i;
		D2D_POINT_2F newPoint = {start_x, start_y};
		tempLine.AdjustProjection(newPoint);
		tempLine.CenterTo(centerPoint);
		tempLine.RotateProjection(start_x, start_y, rotation);
		tempLine.Render("WHITE", 2);
	}

	for(auto& i: player_bullets){i.Render();}
}

void Player::DeadRender()
{
	//SPAWN EXPLOSION ON POINT OF CONTACT
	
	float rotation_variance = -0.2;
	//Rotational value -1.57

	for(auto i: GeometricShapes::player)
	{
		float tempUnitX = cos(rotation + rotation_variance) * sqrt((vecX * vecX) + (vecY * vecY));	
		float tempUnitY = sin(rotation + rotation_variance) * sqrt((vecX * vecX) + (vecY * vecY));

		float new_start_x = start_x + (tempUnitX * death_movement_counter);
		float new_start_y = start_y + (tempUnitY * death_movement_counter);

		Line tempLine = i;
		D2D_POINT_2F newPoint = {new_start_x, new_start_y};
		tempLine.AdjustProjection(newPoint);
		tempLine.CenterTo(centerPoint);
		tempLine.RotateProjection(new_start_x, new_start_y, rotation + (death_movement_counter / 100));
		tempLine.Render("WHITE", 2);
		rotation_variance -= 0.2;
	}
}

D2D1_POINT_2F Player::GetStartPoint(){return D2D1::Point2F(start_x, start_y);}

#endif