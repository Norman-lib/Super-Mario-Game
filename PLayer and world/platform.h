#pragma once
#include "utils.h"
#include "stb_image.h"


using namespace std;

class platform {
	//attributs
	vector<float> pos;
	vector<float> dim;
	GLuint texture;
	

public:

	//Constructer
	platform(vector<float> pos, vector<float> dim);
	//getters
	vector<float> getPos();
	vector<float> getDim();
	//Methods
	void draw();
	void drawWithTexture(const char* filename);
	void loadTexture();

};
