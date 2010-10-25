#pragma once
#include <vector>

using namespace std;

class Node
{
	vector<Node*> nodes;
	
public:
	Node(int);
	~Node(void);

	Node* getNeighbourAt(int);
	void setNeighbourAt(int, Node*);
};

