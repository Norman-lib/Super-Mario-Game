#pragma once

#include <gl/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>;
#include <iostream>
#include <irrKlang.h>
#include"Strecture.h"


using namespace std;

using namespace irrklang;


class Mario {
    vector<float> Position = { -1,0 };
    vector<float> Dimension = { 0.5,0.5 };
	int score=0;
	
	bool textureRight;

public:
	vector<float> velocity = { 0, 0 };
	vector<float> gravity = { 0, -0.5 };
	float jumpForce = 13.0f;
	float moveForce = 2.0f;
	float deltaTime = 0.0f;
	float currentTime = 0.0f;
	float lastTime = 0.0f;
	bool left1;
	bool right1;
	//texture vars;
	unsigned char* marioLeftData;
	int marioWidth, marioHeight, marioNrChannels;
	GLuint textureMarioLeft;
	GLuint textureMarioRight;


	GLfloat playerX = 0.0f;

	
	//variable
	bool jump = false;
	bool canJump = false;
	
	

	Mario();
	int getScore() { return score; }
	void setScore() { score += 1; }
	vector<float> getPosition() { return Position; }
	vector<float> getDimension(){ return Dimension; }
	void UpdatePlayerPosition(vector<float> platformPos, vector<float> platformDim);
	
	void  setPosition(vector<float> pos) { Position = pos; }
	void drawPlayerWithTexture(vector<float> pos, vector<float> dim);
	void displayScore();


	
	

	Struct::Collision checkCollision(vector<float> targetPos, vector<float> targetDims,vector<float> platformPos, vector<float> platformDim);
	bool DetectCollisionWPlayer(vector<float> targetPos, vector<float> targetDims);

	Struct::CollisionSide checkEnemyCollision(vector<float> playerPos, vector<float> playerDim, vector<float> platformPos, vector<float> platformDim);
	void detectGoldCols(vector<vector<float>> goldPos, float radius);
	void detectEnnemyCols(vector<vector<float>> EnnemyPos, vector<float> dim);
};