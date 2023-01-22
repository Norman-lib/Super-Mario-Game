#pragma once
#include "utils.h"
#include "stb_image.h"

using namespace std;

class flag {
	vector<float> pos;
	vector<float> dim;

public:

	//Constructer
	flag(vector<float> pos, vector<float> dim);

	//Methods
	void draw();
	vector<float> getPos();
	vector<float> getDim();
	void drawWithTexture(const char* filename);

};