
#include "point.h"
#include "utils.h"

#include <unordered_set>
#include <ostream>

using namespace std;

#ifndef NODE_H_
#define NODE_H_

class Node;

typedef unordered_set<Node*, KeyHash<Node>, KeyEqual<Node>> set_nodes;

class Node
{
	const Point *point;
	Node *parent;
	const int level;
	set_nodes children;
	set_nodes relatives;
public:
	Node(const Point &point, int level);
	set_nodes GetRelatives() const { return relatives; }
	set_nodes GetChildren() const { return children; }
	Node* GetChild() const { return children.size() > 0 ? *children.begin() : nullptr; }
	Node* GetParent() const { return parent; }
	const Point* GetPoint() const { return point; }
	int GetLevel() const { return level; }
	void AddRelative(Node &rel);
	void AddChild(Node &ch);
	void RemoveChild(Node& ch);
	void SetParent(Node &parent);
	bool operator==(const Node &other) const;
	bool operator!=(const Node &other) const;
	friend ostream& operator<<(ostream &strm, const Node *node);
	friend ostream& operator<<(ostream &strm, const Node &node);
	void Print();
};

set_nodes GetChildren(set_nodes nodes);
set_nodes GetRelatives(set_nodes nodes);
set_nodes GetParent(set_nodes nodes);

#endif
