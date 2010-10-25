#pragma once
#include <vector>
#include "Node.h"

using namespace std;

class Graph
{
	vector<Node*> nodes;

public:
	Graph(int);
	~Graph(void);

	Node* getNodeAt(unsigned int);
	void setNodeAt(unsigned int, Node*);
};

