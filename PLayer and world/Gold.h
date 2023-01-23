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
private:
	vector<float> goldPos;
	vector<float> dim ;
	float radius = 0.5;
	GLuint textureCoins;
public: 
	//Constructeur 
	Gold();
	Gold(vector<float> pos);
	//attributs
	
	//getters
	vector<float> getPosition() { return goldPos; }

	//setters
	void setPosition(vector<float> a) { goldPos = a;  }
	//methodes
	void drawGold(vector<float>);
	void loadTexture(const char* filename); 

};


