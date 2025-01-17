#ifndef GAME_H
#define GAME_H

enum GameState
{
	MainMenu,
	Alive,
	Dead,
	Inventory
};

namespace Game
{
	//Player Management
	Player player;

	//Star Management
	std::vector<SolarSystem> star_systems;

	//Ore Management
	std::vector<Ore> oreBuffer;
	void UpdateOre(float _delta);

	//Astroids Management
	std::vector<Astroid> astroid_list;
	bool astroids_spawning = false;
	const int ASTROID_MAX_COUNT = 10;

	void UpdateAstroids(float _delta);

	//Game Management
	int score;
	Text score_text;
	Text death_text;

	GameState game_state = GameState::Alive;
	GameState prev_state = GameState::Alive;

	void Init();
	void Update(float _delta);
	void Render();
	bool HandleInput(WPARAM _wParam, bool _keyDown);
}

void Game::Init()
{
	player.Create();

	Font::Create();
	GeometricShapes::InitalizePlayerLines();
	GeometricShapes::InitalizeAstroidLines();

	score_text.Create(D2D1::RectF(0,0,100,200), "GREEN");
	death_text.Create(D2D1::RectF(0,0, 300, 300), "WHITE");
	death_text.content = "Press SPACE to respawn";
}

void Game::Update(float _delta)
{
	UpdateSolarSystems(star_systems);
	UpdateAstroids(_delta);
	UpdateOre(_delta);
	InventoryMenu::Tick(_delta);

	if(InventoryMenu::active){
		if(game_state != GameState::Inventory){prev_state = game_state;}
		game_state = GameState::Inventory;
	}
	else{game_state = prev_state;}

	if(game_state == GameState::Alive)
	{
		player.Update(_delta);
		player.CheckAstroidCollisions(astroid_list);

		if(!player.GetAlive()){game_state = GameState::Dead;}
	}

	if(game_state == GameState::Dead)
	{
		player.Update(_delta);
		if(player.GetAlive()){game_state = GameState::Alive;}
	}
}

void Game::UpdateOre(float _delta)
{
	std::vector<Line> lines = GeometricShapes::player;

	if(oreBuffer.size() > 0)
	{
		for(auto& line : lines){
			line.AdjustProjection(player.GetStartPoint());
			line.CenterTo(player.centerPoint);
			line.RotateProjection(player.start_x, player.start_y, player.GetRotation());
		}

		for(int i = 0; i < oreBuffer.size(); i++)
		{
			D2D_POINT_2F ore = D2D1::Point2F(oreBuffer.at(i).GetX() + cameraOffsetX, oreBuffer.at(i).GetY() + cameraOffsetY);

			D2D_POINT_2F nearest = Collisions::NearestPointOnTriangle(ore, lines.at(0).GetOffset(),
																		lines.at(1).GetOffset(),
																		lines.at(2).GetOffset());

			if(Collisions::CircleTriangle(nearest,ore, 8)){
				switch(oreBuffer.at(i).ore_type){
				case Copper:
					score += 1;
					break;
				case Silver:
					score += 10;
					break;
				case Gold:
					score += 100;
					break;
				}

				oreBuffer.erase(oreBuffer.begin() + i);
				break;
			}
		}
	}
}

std::vector<Line> lines = GeometricShapes::player;

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

	//Bullet Collision Detection
	for(auto& bullets: player.player_bullets)
	{
		for(int i = 0; i < astroid_list.size(); i++)
		{
			if(astroid_list.at(i).CheckCollision(bullets.GetX(), bullets.GetY())){
				Astroid* astroid = &astroid_list.at(i);
				Ore newOre;
				newOre.CreateOnAstroid(astroid->GetSize(), astroid->GetX(), astroid->GetY());
				oreBuffer.push_back(newOre);
				astroid->Hit(astroid_list);
				astroid_list.erase(astroid_list.begin() + i);
				bullets.current_life = bullets.MAX_LIFESPAN;
			}
		}
	}
}

bool Game::HandleInput(WPARAM _wParam, bool _keyDown)
{
	if(player.HandleInput(_wParam, _keyDown)){return true;}
	if(InventoryMenu::HandleEvents(_wParam, _keyDown)){return true;}
	return false;
}

void Game::Render()
{
	//Draw the Stars, Astroids and Player
	for(auto stars: Game::star_systems){stars.Render();}
	for(auto astroid : astroid_list){astroid.Render();}
	for(auto ore: oreBuffer){ore.Render();}

	score_text.content = "Score: " + std::to_string(score);
	Font::Render(&score_text);

	if(game_state == GameState::Inventory)
	{
		InventoryMenu::Render();
	}
	if(game_state == GameState::Alive)
	{
		Game::player.Render();
	}

	if(game_state == GameState::Dead)
	{
		Game::player.DeadRender();
		Font::Render(&death_text);
	}


}


#endif