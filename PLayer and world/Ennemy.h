#pragma once
#include "utils.h"

class Enemy {
private:
	vector<float> position;
	vector <float> dimentions;

public:
	Enemy(vector<float> pos, vector<float> dim);
	//getters and setters
	void setPosition(vector<float> f) { position = f; };
	void setDimentions(vector<float> f) { dimentions = f; };
	vector<float> getPosition() { return position; };
	vector<float> getDimentions() { return dimentions; };

	//functions
	void drawEnemy();
	void Move();



};
