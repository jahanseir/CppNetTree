
#include "point.h"
#include "node.h"
#include "utils.h"
#include "metric.h"

#include <unordered_map>

#ifndef POINTLOCATION_H_
#define POINTLOCATION_H_

typedef unordered_map<const Point*, Node*, KeyHash<Point>, KeyEqual<Point>> map_centers;
typedef unordered_map<Node*, set_points, KeyHash<Node>, KeyEqual<Node>> map_cells;

template <class PL>
class NetTree;

template <class PL>
class PointLocation
{
protected:
	NetTree<PL> *tree;
	Metric *metric;
	Node *root;
public:
	PointLocation(NetTree<PL> &tree, vector<const Point*> points)
	{
		this->tree = &tree;
		metric = tree.GetMetric();
		root = tree.GetRoot();
	}
	virtual Node* GetCenter(const Point &point) const = 0;
	virtual float DistToCenter(const Point &point, const Node* nearest = NULL) const = 0;
	virtual void RemovePoint(const Point &point) = 0;
	virtual void AddNode(Node &node) = 0;
	virtual void UpdateOnInsertion(Node &node) = 0;
	virtual void UpdateOnDeletion(Node &node) = 0;
	virtual void UpdateOnSplit(Node &node) = 0;
	virtual ~PointLocation(){};
};

class EagerPointLocation : public PointLocation<EagerPointLocation>
{
	map_centers centermap;
	map_cells innercellmap;
	map_cells outercellmap;
	void TryChangeCenter(const Point &point, Node &newcenter);
	void ChangeCenter(const Point &point, Node &oldcenter, Node &newcenter);
public:
	EagerPointLocation(NetTree<EagerPointLocation> &tree, vector<const Point*> uninserted_points);
	set_points GetInnerCell(set_nodes nodes) const;
	set_points GetInnerCell(Node &node) const { return innercellmap.at(&node); }
	set_points GetOuterCell(set_nodes nodes) const;
	set_points GetOuterCell(Node &node) const { return outercellmap.at(&node); }
	set_points GetCell(set_nodes nodes) const;
	set_points GetCell(Node &node) const;
	Node* GetCenter(const Point &point) const { return centermap.at(&point); }
	float DistToCenter(const Point &point, const Node* nearest = NULL) const;
	void RemovePoint(const Point &point);
	void AddNode(Node &node);
	void UpdateOnInsertion(Node &node);
	void UpdateOnDeletion(Node &node);
	void UpdateOnSplit(Node &node);
};


#endif
