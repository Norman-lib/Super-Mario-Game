#pragma once
#include<vector>
using namespace std;
//for the collision
class Struct {
public:
	enum CollisionSide {
		None,
		Top,
		Bottom,
		Left,
		Right,
	};
	struct Collision {
		CollisionSide side;
		vector<float> pos;
		vector<float> dim;

	};
};
