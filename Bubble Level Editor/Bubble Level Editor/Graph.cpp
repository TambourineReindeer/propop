#include "StdAfx.h"
#include "Graph.h"


Graph::Graph(int numberOfBaseNodes)
{
	nodes = vector<Node>();
	baseNodes = vector<Node*>(numberOfBaseNodes);
}


Graph::~Graph(void)
{
}

Node* Graph::getBaseNodeAt(unsigned int index) {
	if ((index >= 0) && (index < nodes.size())) {
		return baseNodes[index];
	}
	else return NULL;
}

void Graph::setBaseNodeAt(unsigned int index, Node* baseNode){
	if ((index >= 0) && (index < nodes.size())) {
		baseNodes[index] = baseNode;
	}
}
