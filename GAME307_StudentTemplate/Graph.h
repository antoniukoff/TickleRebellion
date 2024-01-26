#pragma once

#include <vector>
#include <map>
#include "Node.h"
#include <iostream>

// DO NOT USE THIS OUTSIDE THIS COURSE

class Graph
{
private:
	// list of nodes
	std::map<int, Node*> nodes;

	// 2D matrix of connection cost (adjacency matrix)
	std::vector< std::vector<float> > costs;
	// i.e. costs[1,3]

public:
	Graph();
	~Graph();

	bool OnCreate(std::vector<Node*> nodes_);
	void OnDestroy();

	int NumNodes();
	Node* GetNode(int label) { return nodes[label]; }
	void AddWeightedConnection(int fromNode, int toNode, float weight);

	std::vector<int> Neighbours(int fromNode);

	std::vector<int> Dijkstra(int start, int goal);

};

