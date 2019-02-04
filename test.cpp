
#include <iostream>
#include <assert.h>
#include <unordered_set>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "metric.hpp"
#include "nettree.hpp"
#include "node.hpp"
#include "point.hpp"
#include "utils.hpp"

using namespace std;

void test_point();
void test_metric();
void test_node();
void test_nettree();
void test_uniform();
float randomfloat(float min, float max);

int main()
{
	srand(time(NULL));
	test_point();
	test_metric();
	test_node();
	test_nettree();
	test_uniform();
	cout<<"Success!";
	return 0;
}

float randomfloat(float min, float max)
{
	return ((float)rand()/RAND_MAX)*(max-min+1)+min;
}

void test_uniform()
{
	int max=10000;
	int min=-max;
	int dim=3;
	int num=2000;
	vector<const BasePoint*> points;
	for(int i=0; i<num; i++)
	{
		vector<float> coords;
		for(int i=0; i<dim; i++)
			coords.push_back(randomfloat(min,max));
		points.push_back(new Point<float>(coords));
	}
	Metric *metric = new Euclidean<float>();
	NetTree<EagerPointLocation> *nt=new NetTree<EagerPointLocation>(metric, 5, 1, 1);
	clock_t t0=clock();
	nt->Construct(points);
	cout<<(float)(clock() - t0) / CLOCKS_PER_SEC<<endl;
	delete nt;
	for(auto p: points)
		delete p;
	delete metric;
}

void test_point()
{
	BasePoint *p1 = new Point<float>(vector<float>(3,5));
	BasePoint *p2 = new Point<float>(vector<float>(3,2));
	assert(p1->Dimension() == 3);
	assert(*p1==Point<float>(vector<float>(3,5)));
	BasePoint *p3 = new Point<float>({5, 8, 5});
	assert(*(float*)((*p3)[1])==8);
	assert(*p3==Point<float>({5,8,5}));
	BasePoint &p4 = *p3 + *p2;
	assert(p4==Point<float>({7,10,7}));
	assert(*p3 - *p2==Point<float>({3,6,3}));
	assert(*p3 * *p2==Point<float>({10,16,10}));
}

void test_metric()
{
	BasePoint *p1 = new Point<float>(vector<float>(9,5));
	BasePoint *p2 = new Point<float>(vector<float>(9,4));
	Metric *m=new Euclidean<float>();
	assert(m->Distance(*p1,*p2)==3);
}

void test_node()
{
	BasePoint *p1 = new Point<float>(vector<float>(15,5));
	BasePoint *p2 = new Point<float>(vector<float>(15,2));
	BasePoint *p3 = new Point<float>(vector<float>(15,2));
	Node *n1 = new Node(*p1, 2);
	Node *n2 = new Node(*p1, 1);
	Node *n3 = new Node(*p2, 1);
	Node *n4 = new Node(*p3, 1);

	// verify == and != operators
	assert(*n1!=*n2);
	assert(*n2==*n2);
	assert(*n3==*n4);

	// verify KeyHash and KeyEqual for Node
	KeyEqual<Node> ne;
	assert(ne(n1,n3)==false);
	assert(ne(n4,n3)==true);
	KeyHash<Node> nhash;
	assert(nhash(n1)!=nhash(n3));
	assert(nhash(n4)==nhash(n3));

	// verify AddChild, GetParent, GetChildren, GetChild, AddRelative, and GetReltives methods
	n1->AddChild(*n2);
	n1->AddChild(*n3);
	assert(n3->GetParent()==n1);
	assert(n1->GetParent()==NULL);
	Node *c = n1->GetChild();
	assert(c==n2 || c==n3);
	delete n4;
	n4 = new Node(*p3, 2);
	assert(n1->GetChildren().find(n4)==n1->GetChildren().end());
	delete n4;
	n4 = new Node(*p3, 1);
	assert(n2->GetRelatives().size()==1);
	assert(n3->GetRelatives().size()==1);
	n2->AddRelative(*n3);
	assert(n1->GetRelatives().size()==1);
	assert(n2->GetRelatives().size()==2);
	assert(n3->GetRelatives().size()==2);
	assert(n3->GetRelatives().find(n2)!=n1->GetRelatives().end());
	BasePoint *p5 = new Point<float>(vector<float>(15,8));
	Node *n5 = new Node(*p5,2);
	n3->SetParent(*n5);
	assert(n3->GetParent()==n5);
	assert(n1->GetChildren().size()==1);
	assert(n1->GetChildren().find(n3)==n1->GetChildren().end());

	// verify general GetChildren and GetRelatives, and GetParent
	BasePoint *p6 = new Point<float>(vector<float>(15,11));
	Node *n6 = new Node(*p6,1);
	n6->SetParent(*n5);
	n1->AddRelative(*n5);
	assert(GetChildren(n1->GetRelatives()).size()==3);
	assert(GetRelatives(n5->GetChildren()).size()==3);
	assert(GetParent(n3->GetRelatives()).size()==2);
	assert(GetParent(n1->GetRelatives()).size()==0);
	delete n1;
	delete n5;
	delete n4;
}

void test_nettree()
{
	Metric *metric = new Euclidean<float>();
	NetTree<EagerPointLocation> *nt=new NetTree<EagerPointLocation>(metric, 5, 2, 1);
	assert(nt->GetTau()==5);
	assert(nt->GetCP()==2);
	assert(nt->GetCC()==1);
	assert(nt->GetCR()==10);
	delete nt;
	nt=new NetTree<EagerPointLocation>(metric, 3, 1, 2, 3);
	BasePoint *p1 = new Point<float>(vector<float>({0,0}));
	vector<const BasePoint*> vect;
	vect.push_back(p1);
	nt->Construct(vect);
	assert(*nt->GetRoot()==Node(*p1, INF_P));
	assert(nt->GetRoot()->GetChildren().size()==1);
	assert(*nt->GetRoot()->GetChild()==Node(*p1, INF_N));

	BasePoint *p2 = new Point<float>(vector<float>({9,0}));
	vect.push_back(p2);
	delete nt;
	nt=new NetTree<EagerPointLocation>(metric, 3, 1, 2, 3);
	nt->Construct(vect);
	nt->Print();

	cout << "==========================================================" << endl;
	cout << "==========================================================" << endl;

	delete nt;
	nt = new NetTree<EagerPointLocation>(metric, 2, 1, 1, 4);
	BasePoint *p3 = new Point<float>({0});
	BasePoint *p4 = new Point<float>({2});
	BasePoint *p5 = new Point<float>({11});
	BasePoint *p6 = new Point<float>({28});
	vector<const BasePoint*> v({p4, p5, p6, p3});
	nt->Construct(v);
	nt->Print();

	cout << "==========================================================" << endl;
	cout << "==========================================================" << endl;

	delete nt;
	nt = new NetTree<EagerPointLocation>(metric, 3, 1, 1, 2);
	BasePoint *p7 = new Point<float>({0});
	BasePoint *p8 = new Point<float>({28});
	BasePoint *p9 = new Point<float>({9});
	BasePoint *p10 = new Point<float>({37});
	BasePoint *p11 = new Point<float>({17});
	vector<const BasePoint*> v2({p8, p9, p10, p11, p7});
	nt->Construct(v2);
	nt->Print();

	cout << "==========================================================" << endl;
	cout << "==========================================================" << endl;
}
