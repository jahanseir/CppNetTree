/*
 * @PointLocation is an abstract base class that defines the skeleton of
 * point location strategies needed to build a net-tree. The template argument is necessary
 * because each @NetTree object should know its point location strategy.
 */

#include <unordered_map>
#include "metric.hpp"
#include "node.hpp"
#include "point.hpp"
#include "utils.hpp"

#ifndef POINTLOCATION_HPP_
#define POINTLOCATION_HPP_

typedef unordered_map<const BasePoint*, Node*> map_centers;
typedef unordered_map<Node*, set_points> map_cells;

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
	PointLocation(NetTree<PL> &tree, vector<const BasePoint*> points)
	{
		this->tree = &tree;
		metric = tree.GetMetric();
		root = tree.GetRoot();
	}
	virtual Node* GetCenter(const BasePoint &point) const = 0;
	virtual float DistToCenter(const BasePoint &point, const Node* nearest = NULL) const = 0;
	virtual void RemovePoint(const BasePoint &point) = 0;
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
	void TryChangeCenter(const BasePoint &point, Node &newcenter);
	void ChangeCenter(const BasePoint &point, Node &oldcenter, Node &newcenter, float todist = 0);
public:
	EagerPointLocation(NetTree<EagerPointLocation> &tree, vector<const BasePoint*> uninserted_points);
	set_points GetInnerCell(set_nodes nodes) const;
	set_points GetInnerCell(Node &node) const { return innercellmap.at(&node); }
	set_points GetOuterCell(set_nodes nodes) const;
	set_points GetOuterCell(Node &node) const { return outercellmap.at(&node); }
	set_points GetCell(set_nodes nodes) const;
	set_points GetCell(Node &node) const;
	Node* GetCenter(const BasePoint &point) const { return centermap.at(&point); }
	float DistToCenter(const BasePoint &point, const Node* nearest = NULL) const;
	void RemovePoint(const BasePoint &point);
	void AddNode(Node &node);
	void UpdateOnInsertion(Node &node);
	void UpdateOnDeletion(Node &node);
	void UpdateOnSplit(Node &node);
};


#endif
