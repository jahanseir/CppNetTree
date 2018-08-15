
#include "metric.h"

#include <math.h>

float Euclidean::Distance(const Point& p1, const Point& p2)
{
	Point pt = (p1 - p2) * (p1 - p2);
	float total = 0;
	for(size_t i = 0; i < pt.Dimension(); i++)
		total += pt[i];
	return sqrt(total);
}

float Euclidean::Distance(const Node& n1, const Node& n2)
{
	return Distance(*n1.GetPoint(), *n2.GetPoint());
}
