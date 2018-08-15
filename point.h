
#include "utils.h"

#include <vector>
#include <string>
#include <unordered_set>

using namespace std;

#ifndef POINT_H_
#define POINT_H_

class Point;

typedef unordered_set<const Point*, KeyHash<Point>, KeyEqual<Point>> set_points;

class Point
{
	const size_t dimension;
	float *coords;
public:
	Point(initializer_list<float> coords);
	Point(vector<float> coords);
	Point(const Point &other);
	size_t Dimension() const { return dimension; }
	float operator[](int index) const { return this->coords[index]; }
	Point& operator-(const Point &other) const;
	Point& operator+(const Point &other) const;
	Point& operator*(const Point &other) const;
	bool operator==(const Point &other) const;
	bool operator!=(const Point &other) const;
	friend ostream& operator<<(ostream &strm, const Point &point);
};

#endif
