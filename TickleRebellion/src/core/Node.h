#pragma once
#include "Vector.h"

class Node
{
private:
	int label;
	MATH::Vec3 position;

public:
	Node(int label_, MATH::Vec3 position_ = {}) :label(label_), position(position_) {}
	~Node() {};

	int GetLabel() { return label; }
	MATH::Vec3 const getPosition() { return position; }
};

