/*
 * @BasePoint is an abstract class defining the generic behaviors and attributes of points in a metric space.
 * @Point is a template class which inherits from @BasePoint. The template argument enables one
 * to customize the underlying space (coordinate system).
 */

#include <vector>
#include <string>
#include <unordered_set>
#include "utils.hpp"

using namespace std;

#ifndef POINT_HPP_
#define POINT_HPP_

class BasePoint{
protected:
	const size_t dimension;
public:
	BasePoint(size_t dimension): dimension(dimension) {};
	size_t Dimension() const { return dimension; }
	virtual void* operator[](int index) const = 0;
	virtual BasePoint& operator-(const BasePoint &other) const = 0;
	virtual BasePoint& operator+(const BasePoint &other) const = 0;
	virtual BasePoint& operator*(const BasePoint &other) const = 0;
	virtual bool operator==(const BasePoint &other) const = 0;
	virtual bool operator!=(const BasePoint &other) const = 0;
	virtual string ToString() const = 0;
	virtual ~BasePoint() {};
	friend ostream& operator<<(ostream &strm, const BasePoint &point)
	{
		strm << point.ToString();
		return strm;
	}
};

typedef unordered_set<const BasePoint*> set_points;

template <class T>
class Point : public BasePoint
{
	T *coords;
public:
	Point(initializer_list<T> coords): BasePoint(coords.size())
	{
		this->coords = new T[dimension];
		copy(coords.begin(), coords.end(), this->coords);
	}

	Point(vector<T> coords): BasePoint(coords.size())
	{
		this->coords = new T[dimension];
		copy(coords.begin(), coords.end(), this->coords);
	}

	Point(T *coords, size_t dimension): BasePoint(dimension)
	{
		this->coords = new T[dimension];
		copy(coords, coords + dimension, this->coords);
	}

	Point(const Point<T> &other): BasePoint(other.dimension)
	{
		this->coords = new T[dimension];
		copy(other.coords, other.coords + dimension, this->coords);
	}

	~Point()
	{
		delete[] coords;
	}

	void* operator[](int index) const { return this->coords + index; }

	BasePoint& operator-(const BasePoint &other) const
	{
		T *coordinates = new T[dimension];
		for(size_t i = 0; i < dimension; i++)
			coordinates[i] = *((T*)((*this)[i])) - *((T*)other[i]);
		Point<T> *pt = new Point<T>(coordinates, dimension);
		return *pt;
	}

	BasePoint& operator+(const BasePoint &other) const
	{
		T *coordinates = new T[dimension];
		for(size_t i = 0; i < dimension; i++)
			coordinates[i] = *((T*)((*this)[i])) + *((T*)other[i]);
		Point<T> *pt = new Point<T>(coordinates, dimension);
		return *pt;
	}

	BasePoint& operator*(const BasePoint &other) const
	{
		T *coordinates = new T[dimension];
		for(size_t i = 0; i < dimension; i++)
			coordinates[i] = *((T*)((*this)[i])) * *((T*)other[i]);
		Point<T> *pt = new Point<T>(coordinates, dimension);
		return *pt;
	}

	bool operator==(const BasePoint &other) const
	{
		for(size_t i = 0; i < dimension; i++)
			if(*((T*)((*this)[i])) != *((T*)other[i]))
				return false;
		return true;
	}

	bool operator!=(const BasePoint &other) const { return !(*this == other); }

	string ToString() const
	{
		string outstr = "";
		for(size_t i = 0; i < dimension; i++)
		{
			if(i > 0)
				outstr += ", ";
			outstr += to_string(coords[i]);
		}
		return outstr;
	}
};

#endif
