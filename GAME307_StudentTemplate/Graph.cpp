#include "Graph.h"

Graph::Graph()
{
}

Graph::~Graph()
{
}

bool Graph::OnCreate(std::vector<Node*> nodes_)
{
	int numNodes = nodes_.size();
	costs.resize(numNodes);

	for (int i = 0; i < numNodes; i++) {


		if (i != nodes_[i]->GetLabel()) {
			std::cerr << "node with label 'i' not in the 'i'th position for " << i << std::endl;
			return false;
		}

		// set up connections
		nodes[i] = nodes_[i];

		costs[i].resize(numNodes);
		for (int j = 0; j < numNodes; j++) {
			// initalizes all connections as 0
			costs[i][j] = 0.0f;

		}
	}

	return true;
}

void Graph::OnDestroy()
{
}

int Graph::NumNodes()
{
	return nodes.size();
}

void Graph::AddWeightedConnection(int fromNode, int toNode, float weight)
{
	costs[fromNode][toNode] = weight;
}

std::vector<int> Graph::Neighbours(int fromNode)
{
	std::vector<int> results = {};
	// loop thru for fromNode
	for (int i = 0; i < NumNodes(); i++) {

		// check if there is connection
		if (costs[fromNode][i] > 0.0f) {
			// return which indices are connected, dont care abt weight rn
			results.push_back(i);
		}
	}
	return results;
}

std::vector<int> Graph::Dijkstra(int start, int goal)
{
	return{};
}
