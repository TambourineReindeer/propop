#include "StdAfx.h"
#include "Node.h"

Node::Node()
{
	nodes = vector<Node*>(12);
}

Node::Node(int numberOfNeighbours)
{
	nodes = vector<Node*>(numberOfNeighbours);
}

Node::~Node(void)
{
}

Node* Node::getNeighbourAt(unsigned int index) {
	if ((index >= 0) && (index < nodes.size())) {
		return nodes[index];
	}
	else return NULL;
}

void Node::setNeighbourAt(unsigned int index, Node* node){
	if ((index >= 0) && (index < nodes.size())) {
		nodes[index] = node;
	}
}

int Node::getType() {
	return type;
}

void Node::setType(int newType) {
	type = newType;
}
