
#include "pointlocation.hpp"

#include <iostream>
#include <math.h>
#include "nettree.hpp"

EagerPointLocation::EagerPointLocation(NetTree<EagerPointLocation> &tree, vector<const BasePoint*> uninserted_points)
: PointLocation<EagerPointLocation>(tree, uninserted_points)
{
	innercellmap[root] = set_points();
	for(const BasePoint *p: uninserted_points)
	{
		centermap[p] = root;
		innercellmap[root].insert(p);
	}
	outercellmap[root] = set_points();
}

set_points EagerPointLocation::GetInnerCell(set_nodes nodes) const
{
	set_points result;
	for(Node *n: nodes)
	{
		set_points inner = GetInnerCell(*n);
		result.insert(inner.begin(), inner.end());
	}
	return result;
}

set_points EagerPointLocation::GetOuterCell(set_nodes nodes) const
{
	set_points result;
	for(auto n: nodes)
	{
		set_points outer = GetOuterCell(*n);
		result.insert(outer.begin(), outer.end());
	}
	return result;
}

set_points EagerPointLocation::GetCell(set_nodes nodes) const
{
	set_points result = GetInnerCell(nodes);
	set_points outer = GetOuterCell(nodes);
	result.insert(outer.begin(), outer.end());
	return result;
}

set_points EagerPointLocation::GetCell(Node &node) const
{
	set_points result = GetInnerCell(node);
	set_points outer = GetOuterCell(node);
	result.insert(outer.begin(), outer.end());
	return result;
}

float EagerPointLocation::DistToCenter(const BasePoint &point, const Node* nearest) const
{
	if(!nearest)
		nearest = centermap.at(&point);
	return metric->Distance(point, *nearest->GetPoint());
}

void EagerPointLocation::RemovePoint(const BasePoint &point)
{
	Node* cent = GetCenter(point);
	innercellmap[cent].erase(&point);
	outercellmap[cent].erase(&point);
	centermap.erase(&point);
}
void EagerPointLocation::AddNode(Node &node)
{
	if(innercellmap.find(&node) == innercellmap.end())
	{
		innercellmap[&node] = set_points();
		outercellmap[&node] = set_points();
	}
}
void EagerPointLocation::UpdateOnInsertion(Node &node)
{
	AddNode(node);
	set_points nearbypoints = GetOuterCell(node.GetParent()->GetRelatives());
	set_points samelevelpoints = GetOuterCell(GetChildren(node.GetParent()->GetRelatives()));
	set_points belowlevelpoints = GetOuterCell(GetChildren(node.GetRelatives()));
	nearbypoints.insert(samelevelpoints.begin(), samelevelpoints.end());
	nearbypoints.insert(belowlevelpoints.begin(), belowlevelpoints.end());
	for(const BasePoint *p: nearbypoints)
		TryChangeCenter(*p, node);
}
void EagerPointLocation::UpdateOnDeletion(Node &node)
{
	Node *par = node.GetParent();
	set_points outercellcopy = GetOuterCell(node);
	// we need a copy of the points in the outer cell of node because the outer cell may change in the for loop
	for(const BasePoint *p: outercellcopy)
	{
		ChangeCenter(*p, node, *par);
		centermap[p] = par;
	}
	outercellmap.erase(&node);
	for(const BasePoint *p: GetInnerCell(node))
		centermap[p] = par;
	innercellmap[par].insert(innercellmap[&node].begin(), innercellmap[&node].end());
	innercellmap.erase(&node);
}
void EagerPointLocation::UpdateOnSplit(Node &node)
{
	AddNode(node);
	if(node.GetLevel() != INF_N)
	{
		for(const BasePoint *p: GetCell(*node.GetParent()))
			TryChangeCenter(*p, node);
	}
}

void EagerPointLocation::TryChangeCenter(const BasePoint &point, Node &newcenter)
{
	Node &oldcenter = *centermap[&point];
	float newdist = metric->Distance(point, *newcenter.GetPoint());
	if(tree->IsRelative(newcenter, point))
	{
		if((oldcenter.GetPoint() == newcenter.GetPoint() &&
				oldcenter.GetLevel() > newcenter.GetLevel()) ||
			(oldcenter.GetPoint() != newcenter.GetPoint() &&
					newdist < metric->Distance(point, *oldcenter.GetPoint())))
			ChangeCenter(point, oldcenter, newcenter);
	}
}

void EagerPointLocation::ChangeCenter(const BasePoint &point, Node &oldcenter, Node &newcenter)
{
	float newdist = metric->Distance(point, *newcenter.GetPoint());
	centermap[&point] = &newcenter;
	if(newdist <= tree->GetCP() * pow(tree->GetTau(), newcenter.GetLevel() - 1) / 2)
		innercellmap[&newcenter].insert(&point);
	else
		outercellmap[&newcenter].insert(&point);
	innercellmap[&oldcenter].erase(&point);
	outercellmap[&oldcenter].erase(&point);
}
