#pragma once
#include <vector>

using namespace std;

class Node
{
	vector<Node*> nodes;
	
public:
	Node(int);
	~Node(void);

	Node* getNeighbourAt(unsigned int);
	void setNeighbourAt(unsigned int, Node*);
};

