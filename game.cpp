#ifndef GAME_H
#define GAME_H

namespace Game
{
	//Player Management
	Player player;

	//Star Management
	std::vector<SolarSystem> star_systems;

	//Ore Management
	//std::vector<Ore> oreBuffer;

	//Astroids Management
	std::vector<Astroid> astroid_list;
	bool astroids_spawning = false;
	const int ASTROID_MAX_COUNT = 10;

	void UpdateAstroids(float _delta);

	//Game Management
	int score;


	void Init();
	void Update(float _delta);
	void Render(ID2D1HwndRenderTarget* _renderTarget);
	bool HandleInput(WPARAM _wParam, bool _keyDown);

	void UpdateStarSystems(float _delta);
	
}

void Game::Init()
{
	//Astroid::InitalizePointList();
	//Player::InitalizePointList();
	player.Create();

	Font::Create();
	GeometricShapes::InitalizePlayerLines();
	GeometricShapes::InitalizeAstroidLines();
}

void Game::Update(float _delta)
{
	player.Update(_delta);
	UpdateStarSystems(_delta);
	UpdateAstroids(_delta);
}

void Game::UpdateAstroids(float _dt)
{
	//Astroid Movement Loop
	for(auto& astroid : astroid_list){astroid.Update(_dt);}
	

	//Check if new astroids can spawn
	int count = 0;
	for(int i = 0; i < astroid_list.size(); i++)
	{
		if(astroid_list.at(i).GetSize() == 3){count++;}
	}
	if(count < Game::ASTROID_MAX_COUNT)
	{
		for(int i = 0; i < ASTROID_MAX_COUNT - count; i++)
		{
			Astroid newAstroid;
			newAstroid.Create();
			astroid_list.push_back(newAstroid);
		}
	}

	//Collision Detection
	for(auto& bullets: player.player_bullets)
	{
		for(int i = 0; i < astroid_list.size(); i++)
		{
			if(astroid_list.at(i).CheckCollision(bullets.x, bullets.y)){
				astroid_list.at(i).Hit(astroid_list);
				astroid_list.erase(astroid_list.begin() + i);
				bullets.current_life = bullets.MAX_LIFESPAN;
			}
		}
	}
}

void Game::UpdateStarSystems(float _dt)
{
	//Update rendering of stars, generate new and remove old.
	//Get the X camera offset divide by 640 then times it by the ceil version should give the start_x closest to the camera
	
	float temp_x = 0;
	float temp_y = 0;

	float screen_width = 640;
	float screen_height = 480;

	if(cameraOffsetX != 0){temp_x = ceil(-cameraOffsetX / screen_width);}
	if(cameraOffsetY != 0){temp_y = ceil(-cameraOffsetY / screen_height);}
	
	if(star_systems.size() < 200)
	{
		for(int y = -1; y <= 1; y++)
		{
			for(int x = -1; x <= 1; x++)
			{
				bool system_found = false;

				for(auto stars: star_systems)
				{
					if(stars.start_x == (temp_x  + x) * screen_width && stars.start_y == (temp_y + y) * screen_height)
					{
						system_found = true;
						break;
					}
				}

				if(!system_found)
				{
					SolarSystem new_star_group;
					new_star_group.AssignXY((temp_x + x) * screen_width, (temp_y + y) * screen_height);
					star_systems.push_back(new_star_group);
				}
			}
		}
	}


	for(int i = 0; i < star_systems.size(); i++){
		if(star_systems.at(i).start_x < -cameraOffsetX - 3000 || star_systems.at(i).start_x > -cameraOffsetX + 3000){
			star_systems.erase(star_systems.begin() + i);
			break;
		}
		if(star_systems.at(i).start_y < -cameraOffsetY - 3000 || star_systems.at(i).start_y > -cameraOffsetX + 3000)
		{
			star_systems.erase(star_systems.begin() + i);
		}
	}
}

bool Game::HandleInput(WPARAM _wParam, bool _keyDown)
{
	if(player.HandleInput(_wParam, _keyDown)){return true;}
	return false;
}

void Game::Render(ID2D1HwndRenderTarget* _renderTarget)
{
	//Draw the Stars, Astroids and Player
	for(auto stars: Game::star_systems){stars.Render(_renderTarget);}
	for(auto astroid : astroid_list){astroid.Render(_renderTarget);}
	Game::player.Render(_renderTarget);
	
	std::string score_text = "Score: " + std::to_string(score);
	Font::Render(_renderTarget, score_text);
}


#endif