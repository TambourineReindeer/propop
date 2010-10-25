#pragma once
#include <vector>

using namespace std;

class Node
{
	vector<Node*> nodes;
	int type;

public:
	Node(int);
	~Node(void);

	Node* getNeighbourAt(unsigned int);
	void  setNeighbourAt(unsigned int, Node*);
	int   getType();
	void  setType(int);
};

