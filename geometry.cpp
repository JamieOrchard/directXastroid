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

#endif