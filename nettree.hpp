
#include <limits>
#include <math.h>
#include <iostream>
#include "metric.hpp"
#include "node.hpp"
#include "pointlocation.hpp"

using namespace std;

#ifndef NETTREE_HPP_
#define NETTREE_HPP_

#define INF_P numeric_limits<int>::max()
#define INF_N numeric_limits<int>::min()

template <class PL>
class NetTree
{
	PL *ploc;
	Metric *metric;
	Node *root;
	vector<const BasePoint*> points;
	const float tau;
	const float c_p;
	const float c_c;
	const float c_r;

	void SetRoot(const BasePoint &p)
	{
		root = new Node(p, INF_P);
		Node *child = new Node(p, INF_N);
		child->SetParent(*root);
	}

	void UpdateRelatives(Node &node, Node &closest)
	{
		for(auto other: GetChildren(closest.GetParent()->GetRelatives()))
			if(IsRelative(node, *other))
			{
				if(other->GetLevel() < node.GetLevel())
					other = Split(other, node.GetLevel());
				node.AddRelative(*other);
			}
	}

	void UpdateChildren(Node &node)
	{
		// we delete nodes at the end because deletion of a node in the for loop causes a dangling reference
		vector<Node*> nodestodelete;
		for(auto other: GetChildren(node.GetRelatives()))
			if(other->GetParent() &&
					metric->Distance(node, *other) < metric->Distance(*other, *other->GetParent()))
			{
				Node *oldpar = other->GetParent();
				node.AddChild(*other);
				Node *child = oldpar->GetChild();
				if(oldpar->GetChildren().size() == 1 &&
						child->GetChildren().size() == 1 &&
						child->GetRelatives().size() == 1)
				{
					ploc->UpdateOnDeletion(*child);
					oldpar->AddChild(*child->GetChild());
					oldpar->RemoveChild(*child);
					nodestodelete.push_back(child);
				}
			}
		for(auto n: nodestodelete)
		{
			delete n;
			n = nullptr;
		}
	}

	void UpdateParent(Node &node, Node &closest)
	{
		pair<float, Node*> par = Nearest<Node, set_nodes>(metric, node, closest.GetParent()->GetRelatives());
		if(par.first == metric->Distance(node, *closest.GetParent()))
			par.second = closest.GetParent();
		Node *child = par.second->GetChild();
		if(child->GetLevel() < node.GetLevel())
		{
			child = Split(par.second, node.GetLevel());
			if(IsRelative(node, *child))
				node.AddRelative(*child);
		}
		node.SetParent(*par.second);
	}

	Node* Promote(Node &node)
	{
		Node *newnode = new Node(*node.GetPoint(), node.GetLevel() + 1);
		Update(*newnode, *node.GetParent());
		return newnode;
	}

	void Update(Node &node, Node &closest)
	{
		if(closest.GetParent()->GetLevel() > closest.GetLevel() + 1)
			Split(&closest, closest.GetLevel() + 1);
		UpdateRelatives(node, closest);
		UpdateParent(node, closest);
		UpdateChildren(node);
		ploc->UpdateOnInsertion(node);
	}

	Node* Split(Node *node, int level)
	{
		if(node->GetLevel() == level)
			return node;
		else if(node->GetLevel() < level)
			node = node->GetParent();
		if(node->GetLevel() < level)
			throw invalid_argument("received level is incorrect");
		Node *newnode = new Node(*node->GetPoint(), level);
		Node *ch = node->GetChild();
		if(ch)
		{
			node->RemoveChild(*ch);
			newnode->AddChild(*ch);
		}
		newnode->SetParent(*node);
		ploc->UpdateOnSplit(*newnode);
		return newnode;
	}

public:
	NetTree(Metric *metric, float tau, float c_p, float c_c, float c_r = 0)
			: ploc(nullptr), metric(metric), root(nullptr), tau(tau), c_p(c_p), c_c(c_c),
			  c_r(c_r == 0 ? (2 * c_c * tau / (tau - 4)): c_r) {};

	void Construct(vector<const BasePoint*> points)
	{
		const BasePoint &lastpoint = *points.back();
		points.pop_back();
		this->points.push_back(&lastpoint);
		SetRoot(lastpoint);
		ploc = new PL(*this, points);
		ploc->AddNode(*root->GetChild());
		for(const BasePoint *p: points)
		{
			Insert(*p);
		}
	}

	void Insert(const BasePoint &point, Node *close = nullptr)
	{
		Node *closest = close != nullptr ? close : ploc->GetCenter(point);
		float dist = metric->Distance(point, *closest->GetPoint());
		ploc->RemovePoint(point);
		int level = MinLevelRelative(point, *closest->GetPoint());
		if(dist <= c_p * pow(tau, level))
			level--;
		if(level < closest->GetLevel())
		{
			if(closest->GetChild()->GetLevel() < level)
				closest = Split(closest, level);
			else
				for(auto n: closest->GetChildren())
					if(*n->GetPoint() == *closest->GetPoint())
					{
						closest = n;
						break;
					}
		}
		Node *node = new Node(point, level);
		Update(*node, *closest);
		Node *lowest = node;
		if(node->GetChildren().size() > 0)
		{
			lowest = new Node(point, level - 1);
			set_nodes ch = node->GetChildren();
			if(ch.find(lowest) == ch.end())
			{
				lowest->SetParent(*node);
				ploc->UpdateOnSplit(*lowest);
			}
		}
		Split(lowest, INF_N);
		while(!IsCovered(*node))
			node = Promote(*node);
	}

	bool IsCovered(const Node &node) const
	{
		return metric->Distance(*node.GetPoint(), *node.GetParent()->GetPoint()) <=
													c_c * pow(tau, node.GetLevel() + 1);
	}

	bool IsRelative(const Node &node, const BasePoint &point) const
	{
		return metric->Distance(*node.GetPoint(), point) <= c_r * pow(tau, node.GetLevel());
	}

	bool IsRelative(const Node &n1, const Node &n2) const
	{
		return IsRelative(n1, *n2.GetPoint());
	}

	int MinLevelRelative(const BasePoint &p1, const BasePoint &p2) const
	{
		return ceil(log(metric->Distance(p1, p2) / c_r) / log(tau));
	}

	vector<const BasePoint*> GetPoints() const { return points; }

	Metric* GetMetric() const { return metric; }

	Node* GetRoot() const { return root; }

	float GetTau() const { return tau; }

	float GetCP() const { return c_p; }

	float GetCC() const { return c_c; }

	float GetCR() const { return c_r; }

	void Print() const { this->root->Print(); }
};


#endif
