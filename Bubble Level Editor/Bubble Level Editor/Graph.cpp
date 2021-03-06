#include "StdAfx.h"
#include "Graph.h"

Graph::Graph()
{
	nodes = vector<Node*>();
	baseNodes = vector<Node*>();
}

Graph::Graph(int numberOfBaseNodes)
{
	nodes = vector<Node*>();
	baseNodes = vector<Node*>(numberOfBaseNodes);
}

Graph::~Graph(void)
{
}

Node* Graph::createNode() {
	Node* n = new Node(6);
	nodes.push_back(n);
	return n;
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
