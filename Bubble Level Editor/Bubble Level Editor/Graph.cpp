#include "StdAfx.h"
#include "Graph.h"


Graph::Graph(int numberOfNodes)
{
	nodes = vector<Node*>(numberOfNodes);
}


Graph::~Graph(void)
{
}

Node* Graph::getNodeAt(unsigned int index) {
	if ((index >= 0) && (index < nodes.size())) {
		return nodes[index];
	}
	else return NULL;
}

void Graph::setNodeAt(unsigned int index, Node* node){
	if ((index >= 0) && (index < nodes.size())) {
		nodes[index] = node;
	}
}
