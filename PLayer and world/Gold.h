#pragma once
#include <gl/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <string>
#include <vector>
#include <irrKlang.h>

using namespace std;
using namespace irrklang;


class Gold {
public: 
	//Constructeur 
	Gold();
	//attributs
	vector<float> goldPos;
	float radius = 0.5;
	//getters
	vector<float> getPosition() { return goldPos; }

	//setters
	void setPosition(vector<float> a) { goldPos = a;  }
	//methodes
	void drawGold(vector<float>);

};


