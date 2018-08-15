
#include "metric.h"
#include "point.h"
#include "node.h"
#include "utils.h"
#include "nettree.h"

#include <iostream>
#include <assert.h>
#include <unordered_set>
#include <stdio.h>

using namespace std;

void test_point();
void test_metric();
void test_node();
void test_nettree();


int main()
{
	test_point();
	test_metric();
	test_node();
	test_nettree();
	cout<<"Success!";
	return 0;
}

void test_point()
{
	Point p1(vector<float>(3,5));
	Point p2(vector<float>(3,2));
	assert(p1.Dimension() == 3);
	assert(p1==Point(vector<float>(3,5)));
	Point p3({5, 8, 5});
	assert(p3[1]==8);
	assert(p3==Point({5,8,5}));
	Point p4 = p3+p2;
	assert(p4==Point({7,10,7}));
	assert(p3-p2==Point({3,6,3}));
	assert(p3*p2==Point({10,16,10}));
}

void test_metric()
{
	Point p1(vector<float>(9,5));
	Point p2(vector<float>(9,4));
	Metric *m=new Euclidean();
	assert(m->Distance(p1,p2)==3);
}

void test_node()
{
	Point p1(vector<float>(15,5));
	Point p2(vector<float>(15,2));
	Point p3(vector<float>(15,2));
	Node n1(p1, 2);
	Node n2(p1, 1);
	Node n3(p2, 1);
	Node *n4 = new Node(p3, 1);

	// verify == and != operators
	assert(n1!=n2);
	assert(n2==n2);
	assert(n3==*n4);

	// verify KeyHash and KeyEqual for Node
	Node *np1=&n1;
	Node *np3=&n3;
	Node *np4=n4;
	KeyEqual<Node> ne;
	assert(ne(np1,np3)==false);
	assert(ne(np4,np3)==true);
	KeyHash<Node> nhash;
	assert(nhash(np1)!=nhash(np3));
	assert(nhash(np4)==nhash(np3));

	// verify AddChild, GetParent, GetChildren, GetChild, AddRelative, and GetReltives methods
	n1.AddChild(n2);
	n1.AddChild(n3);
	assert(n3.GetParent()==&n1);
	assert(n1.GetParent()==NULL);
	assert(n1.GetChildren().find(n4)!=n1.GetChildren().end());
	Node *c = n1.GetChild();
	assert(*c==n2 || *c==n3);
	delete n4;
	n4 = new Node(p3, 2);
	assert(n1.GetChildren().find(n4)==n1.GetChildren().end());
	delete n4;
	n4= new Node(p3, 1);
	assert(n2.GetRelatives().size()==1);
	assert(n3.GetRelatives().size()==1);
	n2.AddRelative(n3);
	assert(n1.GetRelatives().size()==1);
	assert(n2.GetRelatives().size()==2);
	assert(n3.GetRelatives().size()==2);
	assert(n2.GetRelatives().find(n4)!=n1.GetRelatives().end());
	assert(n3.GetRelatives().find(&n2)!=n1.GetRelatives().end());
	Point p5(vector<float>(15,8));
	Node n5(p5,2);
	n3.SetParent(n5);
	assert(n3.GetParent()==&n5);
	assert(n1.GetChildren().size()==1);
	assert(n1.GetChildren().find(&n3)==n1.GetChildren().end());

	// verify general GetChildren and GetRelatives, and GetParent
	Point p6(vector<float>(15,11));
	Node n6(p6,1);
	n6.SetParent(n5);
	n1.AddRelative(n5);
	assert(GetChildren(n1.GetRelatives()).size()==3);
	assert(GetRelatives(n5.GetChildren()).size()==3);
	assert(GetParent(n3.GetRelatives()).size()==2);
	assert(GetParent(n1.GetRelatives()).size()==0);
}

void test_nettree()
{
	Metric *metric = new Euclidean();
	NetTree<EagerPointLocation> *nt=new NetTree<EagerPointLocation>(metric, 5, 2, 1);
	assert(nt->GetTau()==5);
	assert(nt->GetCP()==2);
	assert(nt->GetCC()==1);
	assert(nt->GetCR()==10);
	nt=new NetTree<EagerPointLocation>(metric, 3, 1, 2, 3);
	Point p1(vector<float>({0,0}));
	vector<const Point*> vect;
	vect.push_back(&p1);
	nt->Construct(vect);
	assert(*nt->GetRoot()==Node(p1, INF_P));
	assert(nt->GetRoot()->GetChildren().size()==1);
	assert(*nt->GetRoot()->GetChild()==Node(p1, INF_N));

	Point p2(vector<float>({9,0}));
	vect.push_back(&p2);
	nt=new NetTree<EagerPointLocation>(metric, 3, 1, 2, 3);
	nt->Construct(vect);
	nt->Print();

	cout << "==========================================================" << endl;
	cout << "==========================================================" << endl;

	nt = new NetTree<EagerPointLocation>(metric, 2, 1, 1, 4);
	Point p3({0});
	Point p4({2});
	Point p5({11});
	Point p6({28});
	vector<const Point*> v({&p4, &p5, &p6, &p3});
	nt->Construct(v);
	nt->Print();

	cout << "==========================================================" << endl;
	cout << "==========================================================" << endl;

	nt = new NetTree<EagerPointLocation>(metric, 3, 1, 1, 2);
	Point p7({0});
	Point p8({28});
	Point p9({9});
	Point p10({37});
	Point p11({17});
	vector<const Point*> v2({&p8, &p9, &p10, &p11, &p7});
	nt->Construct(v2);
	nt->Print();

	cout << "==========================================================" << endl;
	cout << "==========================================================" << endl;
}
