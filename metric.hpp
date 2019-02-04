/*
 * Defines an abstract base class @Metric used to measure the distance between two points or nodes.
 * @Euclidean and @Manhattan subclasses implement their own distance functions.
 */

#include <utility>
#include <limits>
#include <stdexcept>
#include <math.h>

#include "node.hpp"
#include "point.hpp"

using namespace std;

#ifndef METRIC_HPP_
#define METRIC_HPP_

class Metric
{
public:
	virtual float Distance(const BasePoint &p1, const BasePoint &p2) = 0;
	float Distance(const Node &n1, const Node &n2)
	{
		return Distance(*n1.GetPoint(), *n2.GetPoint());
	}
	virtual ~Metric(){};
};

template <class T>
class Euclidean : public Metric
{
public:
	float Distance(const BasePoint &p1, const BasePoint &p2)
	{
		float total = 0;
		for(size_t i = 0; i < p1.Dimension(); i++)
			total += pow(*((T*)p1[i])-*((T*)p2[i]),2);
		return sqrt(total);
	}
};

template <class T>
class Manhattan : public Metric
{
public:
	float Distance(const BasePoint &p1, const BasePoint &p2)
	{
		float total = 0;
		for(size_t i = 0; i < p1.Dimension(); i++)
			total += *((T*)p1[i])-*((T*)p2[i]);
		return total;
	}
};

template <typename T1, typename T2>
pair<float, T1*> Nearest(Metric *metric, T1 &item, T2 collection)
{
	if(collection.size() == 0)
		throw invalid_argument("received empty collection");
	T1 *closest = nullptr;
	float closestdist = numeric_limits<float>::max();
	for(T1 *obj: collection)
	{
		float dist = metric->Distance(item, *obj);
		if(dist < closestdist)
		{
			closest = obj;
			closestdist = dist;
		}
	}
	return make_pair(closestdist, closest);
}


#endif
