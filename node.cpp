
#include "node.h"
#include "utils.h"

#include <iostream>
#include <queue>
#include <map>

using namespace std;

Node::Node(const Point &point, int level): point(&point), level(level)
{
	parent = nullptr;
	relatives.insert(this);
}

ostream& operator<<(ostream &strm, const Node &node)
{
	strm << &node;
	return strm;
}

// required for set and dictionaries because in those cases keys are pointers of Node
// and nodes with the same points and levels should be evaluated the same

ostream& operator<<(ostream &strm, const Node *node)
{
	if(node==nullptr)
		strm << "NULL";
	else
		strm << "Node -> point: " << *node->point << ", level: " << node->level;
	return strm;
}

void Node::AddRelative(Node &rel)
{
	this->relatives.insert(&rel);
	rel.relatives.insert(this);
}

void Node::AddChild(Node &ch)
{
	if(ch.parent)
		ch.parent->children.erase(&ch);
	children.insert(&ch);
	ch.parent = this;
}

void Node::RemoveChild(Node &ch)
{
	children.erase(&ch);
	ch.parent = nullptr;
}

void Node::SetParent(Node &par)
{
	par.AddChild(*this);
}

bool Node::operator==(const Node &other) const
{
	return *(this->point) == *(other.point) && this->level == other.level;
}

bool Node::operator!=(const Node &other) const
{
	return !(*this == other);
}

void Node::Print()
{
	map<int,vector<Node*>,greater<int>> dict;
	queue<Node*> q;
	q.push(this);
	while(!q.empty())
	{
		Node *front = q.front();
		dict[front->level].push_back(front);
		for(Node *ch: front->children)
			q.push(ch);
		q.pop();
	}
	cout << "----------------------" << endl;
	for(auto kvp: dict)
	{
		cout << "level: " << kvp.first << endl;
		for(Node *node: kvp.second)
			cout << node << " with par " << node->GetParent() << endl;
		cout << "----------------------" << endl;
	}
}

set_nodes GetChildren(set_nodes nodes)
{
	set_nodes result;
	for (Node *node: nodes)
		for (Node *ch: node->GetChildren())
			result.insert(ch);
	return result;
}

set_nodes GetRelatives(set_nodes nodes)
{
	set_nodes result;
	for (Node *node: nodes)
		for (Node *ch: node->GetRelatives())
			result.insert(ch);
	return result;
}

set_nodes GetParent(set_nodes nodes)
{
	set_nodes result;
	for (Node *node: nodes)
	{
		Node *par = node->GetParent();
		if(par)
			result.insert(par);
	}
	return result;
}

