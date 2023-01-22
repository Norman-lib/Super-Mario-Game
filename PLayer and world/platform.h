#pragma once
#include "utils.h"
#include "stb_image.h"


using namespace std;

class platform {
	vector<float> pos;
	vector<float> dim;

public:

	//Constructer
	platform(vector<float> pos, vector<float> dim);

	//Methods
	void draw();
	void drawWithTexture(const char* filename);
	vector<float> getPos();
	vector<float> getDim();

};
