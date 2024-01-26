#pragma once


class Node
{
private:
	int label;

public:
	Node(int label_) {
		label = label_;
	}
	~Node() {};


	int GetLabel() { return label; }


};

