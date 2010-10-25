#include "StdAfx.h"
#include "Node.h"

Node::Node(int numberOfNeighbours)
{
	nodes = vector<Node*>(numberOfNeighbours);
}


Node::~Node(void)
{
}

Node* getNeighbourAt(int index) {
	if ((index >= 0) && (index < nodes.size())) {
		return nodes[index];
	}
	else return NULL;
}

void setNeighbourAt(int index, Node* node){

}