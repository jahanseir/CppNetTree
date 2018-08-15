
#include "point.h"
#include "node.h"

#include <utility>
#include <limits>
#include <stdexcept>

using namespace std;

#ifndef METRIC_H_
#define METRIC_H_

class Metric
{
public:
	virtual float Distance(const Point& p1, const Point& p2) = 0;
	virtual float Distance(const Node& n1, const Node& n2) = 0;
	virtual ~Metric(){};
};

class Euclidean : public Metric
{
public:
	float Distance(const Point& p1, const Point& p2);
	float Distance(const Node& n1, const Node& n2);
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
