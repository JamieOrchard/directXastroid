#ifndef GEOMETRY_H
#define GEOMETRY_H

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

	D2D_POINT_2F GetOffset();
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

D2D_POINT_2F Line::GetOffset()
{
	return D2D1::Point2F(start.x, start.y);
}

namespace GeometricShapes
{
	std::vector<Line> player;
	std::vector<Line> astroids_size_1;
	std::vector<Line> astroids_size_2;
	std::vector<Line> astroids_size_3;

	bool GetCordFromFile(FILE* _file, std::vector<D2D_POINT_2U>& _cords);
	void GeneratePaths(std::vector<D2D_POINT_2U> &_cords, std::vector<Line>& _paths);
	void InitalizePlayerLines();
	void InitalizeAstroidLines();
}

bool GeometricShapes::GetCordFromFile(FILE* _file, std::vector<D2D_POINT_2U>& _cords)
{
	if(_file == NULL){return false;}

	int x = 0;
	int y = 0;
	while(fscanf(_file, "%u %u", &x, &y) != EOF){
		D2D_POINT_2U cord = D2D1::Point2U(x,y);
		_cords.push_back(cord);
	}

	return true;
}

void GeometricShapes::GeneratePaths(std::vector<D2D_POINT_2U> &_cords, std::vector<Line>& _paths)
{
	for(int i = 0; i < _cords.size(); i++)
	{
		Line new_line;
		new_line.start = D2D1::Point2F(_cords.at(i).x, _cords.at(i).y);

		if(i + 1 >= _cords.size()){
			new_line.end = D2D1::Point2F(_cords.at(0).x, _cords.at(0).y);
		}
		else{
			new_line.end = D2D1::Point2F(_cords.at(i + 1).x, _cords.at(i + 1).y);
		}

		_paths.push_back(new_line);
	}
}

void GeometricShapes::InitalizePlayerLines()
{
	FILE *file;
	file = fopen("DATA\\player.txt", "r");

	std::vector<D2D_POINT_2U>cord_list;
	if(!GetCordFromFile(file, cord_list)){
		printf("ERROR: Failed to open file player.txt\n");
		return;
	}
	GeneratePaths(cord_list, player);
	fclose(file);
}

void GeometricShapes::InitalizeAstroidLines()
{
	std::vector<D2D_POINT_2U>cord_list;

	FILE *file;
	file = fopen("DATA\\astroid_3.txt", "r");
	if(!GetCordFromFile(file, cord_list)){
		printf("ERROR: Failed to open file astroid_3.txt\n");
		return;
	}
	GeneratePaths(cord_list, astroids_size_3);
	cord_list.clear();
	fclose(file);

	file = fopen("DATA\\astroid_2.txt", "r");
	if(!GetCordFromFile(file, cord_list)){
		printf("ERROR: Failed to open file astroid_3.txt\n");
		return;
	}
	GeneratePaths(cord_list, astroids_size_2);
	cord_list.clear();
	fclose(file);

	file = fopen("DATA\\astroid_1.txt", "r");
	if(!GetCordFromFile(file, cord_list)){
		printf("ERROR: Failed to open file astroid_3.txt\n");
		return;
	}
	GeneratePaths(cord_list, astroids_size_1);
	fclose(file);
}

namespace Collisions
{
	float DotProduct(D2D_POINT_2F _a, D2D_POINT_2F _b);
	D2D_POINT_2F NearestPointOnTriangle(D2D_POINT_2F _point, D2D_POINT_2F _a, D2D_POINT_2F _b, D2D_POINT_2F _c);
	bool CircleTriangle(D2D_POINT_2F _p, D2D_POINT_2F _c, float _r);
}

float Collisions::DotProduct(D2D_POINT_2F _a, D2D_POINT_2F _b)
{
	return (_a.x * _b.x) + (_a.y * _b.y);
}

//I think this is a little over the top
D2D_POINT_2F Collisions::NearestPointOnTriangle(D2D_POINT_2F _p, D2D_POINT_2F _a, D2D_POINT_2F _b, D2D_POINT_2F _c)
{
	D2D_POINT_2F ab = D2D1::Point2F(_b.x - _a.x, _b.y - _a.y);
	D2D_POINT_2F ac = D2D1::Point2F(_c.x - _a.x, _c.y - _a.y);

	//Vertex Region Outside of A
	D2D_POINT_2F ap = D2D1::Point2F(_p.x - _a.x, _p.y - _a.y);
	float d1 = DotProduct(ab, ap);
	float d2 = DotProduct(ac, ap);
	if(d1 <= 0 && d2 <= 0){return _a;}

	//Vertex Region Outside of B
	D2D_POINT_2F bp = D2D1::Point2F(_p.x - _b.x, _p.y - _b.y);
	float d3 = DotProduct(ab, bp);
	float d4 = DotProduct(ac, bp);
	if(d3 >= 0 && d4 <= d3){return _b;}

	//Edge Region AB
	if(d1 >= 0 && d3 <= 0 && (d1 * d4) - (d3 * d2) <= 0){
		float v = d1 / (d1 - d3);
		return D2D1::Point2F(_a.x + (ab.x * v), _a.y + (ab.y * v));
	}

	//Vertex Region Outside of C
	D2D_POINT_2F cp = D2D1::Point2F(_p.x - _c.x, _p.y - _c.y);
	float d5 = DotProduct(ab, cp);
	float d6 = DotProduct(ac, cp);
	if(d6 >= 0 && d5 <= d6){return _c;}

	//Edge Region AC
	if(d2 >= 0 && d6 <= 0 && (d5 * d2) - (d1 * d6) <= 0)
	{
		float w = d2/(d2 - d6);
		return D2D1::Point2F(_a.x + (ac.x * w), _a.y + (ac.y * w));
	}

	//Edge Region BC
	if((d3 * d6) - (d5 * d4) <= 0){
		float d43 = d4 - d3;
		float d56 = d5 - d6;
		if(d43 >= 0 && d56 >= 0){
			float w = d43/(d43 + d56);
			return D2D1::Point2F(_b.x + (_c.x - _b.x)*w, _b.y + (_c.y - _b.y)*w);
		}
	}

	//Inside Face Region
	return _p;
}

bool Collisions::CircleTriangle(D2D_POINT_2F _p, D2D_POINT_2F _c, float _r)
{
	float dx = _p.x - _c.x;
	float dy = _p.y - _c.y;

	return (dx*dx + dy*dy <= _r*_r);
}

#endif