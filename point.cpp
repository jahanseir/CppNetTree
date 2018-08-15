
#include "point.h"
#include "utils.h"

#include <iostream>

using namespace std;

Point::Point(initializer_list<float> coords): dimension(coords.size())
{
	this->coords = new float[dimension];
	copy(coords.begin(), coords.end(), this->coords);
}

Point::Point(vector<float> coords): dimension(coords.size())
{
	this->coords = new float[dimension];
	copy(coords.begin(), coords.end(), this->coords);
}

Point::Point(const Point &other): dimension(other.dimension)
{
	this->coords = new float[dimension];
	copy(other.coords, other.coords + dimension, this->coords);
}

ostream& operator<<(ostream &strm, const Point &point)
{
	for(size_t i = 0; i < point.Dimension(); i++)
	{
		if(i > 0)
			strm << ", ";
		strm << point[i];
	}
	return strm;
}

Point& Point::operator-(const Point &other) const
{
	vector<float> coordinates(Dimension(), 0);
	for(size_t i = 0; i < this->Dimension(); i++)
		coordinates[i] = (*this)[i] - other[i];
	Point *pt = new Point(coordinates);
	return *pt;
}

Point& Point::operator+(const Point &other) const
{
	vector<float> coordinates(Dimension(), 0);
	for(size_t i = 0; i < this->Dimension(); i++)
		coordinates[i] = (*this)[i] + other[i];
	Point *pt = new Point(coordinates);
	return *pt;
}

Point& Point::operator*(const Point &other) const
{
	vector<float> coordinates(Dimension(), 0);
	for(size_t i = 0; i < this->Dimension(); i++)
		coordinates[i] = (*this)[i] * other[i];
	Point *pt = new Point(coordinates);
	return *pt;
}

bool Point::operator==(const Point &other) const
{
	for(size_t i = 0; i < this->Dimension(); i++)
		if((*this)[i] != other[i])
			return false;
	return true;
}

bool Point::operator!=(const Point &other) const
{
	return !(*this == other);
}
