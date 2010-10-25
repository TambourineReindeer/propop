#pragma once
#include <vector>
#include "Node.h"

using namespace std;

class Graph
{
	vector<Node> nodes;
	vector<Node*> baseNodes;

public:
	Graph(int);
	~Graph(void);

	Node* getBaseNodeAt(unsigned int);
	void setBaseNodeAt(unsigned int, Node*);
};

