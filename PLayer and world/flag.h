#pragma once
#include "utils.h"
#include "stb_image.h"

using namespace std;

class flag {
	vector<float> pos;
	vector<float> dim;
	GLuint texture;

public:

	//Constructer
	flag(vector<float> pos, vector<float> dim);

	//Methods
	vector<float> getPos();
	vector<float> getDim();
	void drawWithTexture();
	void loadTexture();

};