#pragma once

#include "utils.h"
#include "platform.h"

class Enemy {
private:
	vector<float> position;
	vector <float> dimentions;
	vector<float> speed;
	float deltaTime;
	float moveForce;
	const char * texturePath;
	GLuint texture;


public:
	Enemy(vector<float> pos, vector<float> dim);
	//getters and setters
	void setPosition(vector<float> f) { position = f; };
	void setDimentions(vector<float> f) { dimentions = f; };
	vector<float> getPosition() { return position; };
	vector<float> getDimentions() { return dimentions; };
	void setDeltaTime(float f) { deltaTime = f; };
	float getDeltaTime() { return deltaTime; };
	void setTexture(GLuint tex) { texture = tex; };
	GLuint getTexture() { return texture; };
	const char* getTexturePath() { return texturePath; };


	//functions
	void drawEnemy();
	void Move(platform*);
	void loadTexture();



};

