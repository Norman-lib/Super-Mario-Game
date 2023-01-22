#pragma once
#include "utils.h"


using namespace std;

class platform {
	vector<float> pos;
	vector<float> dim;

public:

	//Constructer
	platform(vector<float> pos, vector<float> dim);

	//Methods
	void draw();
	vector<float> getPos();
	vector<float> getDim();

};
