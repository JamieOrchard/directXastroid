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

namespace GeometricShapes
{
	std::vector<Line> player;
	std::vector<Line> astroids_size_1;
	std::vector<Line> astroids_size_2;
	std::vector<Line> astroids_size_3;

	bool GetCordFromFile(FILE* _file, std::vector<D2D_POINT_2U>& _cords);
	void InitalizePlayerLines();
	void InitalizeAstroidLines();
}

bool GeometricShapes::GetCordFromFile(FILE* _file, std::vector<D2D_POINT_2U>& _cords)
{
	if(_file == NULL){
		return false;
	}

	int x = 0;
	int y = 0;
	while(fscanf(_file, "%u %u", &x, &y) != EOF){
		D2D_POINT_2U cord = D2D1::Point2U(x,y);
		_cords.push_back(cord);
	}

	return true;
}

void GeometricShapes::InitalizePlayerLines()
{
	FILE *file;
	file = fopen("DATA\\player.txt", "r");

	std::vector<D2D_POINT_2U>cord_list;
	GetCordFromFile(file, cord_list);	
	
	//Generate Paths
	for(int i = 0; i < cord_list.size(); i++)
	{
		Line new_line;
		new_line.start = D2D1::Point2F(cord_list.at(i).x, cord_list.at(i).y);

		if(i + 1 >= cord_list.size()){
			new_line.end = D2D1::Point2F(cord_list.at(0).x, cord_list.at(0).y);
		}
		else{
			new_line.end = D2D1::Point2F(cord_list.at(i + 1).x, cord_list.at(i + 1).y);
		}

		player.push_back(new_line);
	}

	fclose(file);
}

void GeometricShapes::InitalizeAstroidLines()
{

}

#endif