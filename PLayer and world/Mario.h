#pragma once

#include <gl/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>;
#include <iostream>
#include <irrKlang.h>
#include"Strecture.h"
#include "platform.h"
/// <summary>
/// La class Mario : permet de créer et manipuler le joueur principal
/// </summary>
using namespace std;

using namespace irrklang;


class Mario {
    /// <summary>
    /// Les attributs du joueur: Position (on 'est en 2D on a besoin de X et de Y du joueur ),les  dimensions (pour les collisions avec les autres objets et pour la texture)
    /// </summary>
    vector<float> Position = { -1,0 };
    vector<float> Dimension = { 0.5,0.5 };
	int score=0;
	

public:
	/// <summary>
	/// Attributs en public (ils serront utilisées et modifiés par les autres classes):
	///		velocity : direction de déplacement  du joueur
	///		gravity : vecteur simulant la gravity qui serra appliqué sur le joueur 
	///		jumpForce : quantifié combien le joueur jump
	///		moveForce : la vitesse de déplacement du joueur
	/// 
	/// </summary>
	vector<float> velocity = { 0, 0 };
	vector<float> gravity = { 0, -0.5 };
	float jumpForce = 13.0f;
	float moveForce = 2.0f;
	float deltaTime = 0.0f;
	float currentTime = 0.0f;
	float lastTime = 0.0f;
	bool left1;
	bool right1;
	bool isLastPositionLeft;
	bool gameOver;
	/// <summary>
	/// Attributs de tecture contient les dimensions de mario et les textures de gauche ,droite et de mort lorsque le joueuer marche à gauche ou à  droite ou s'il mort
	/// </summary>
	unsigned char* marioLeftData;
	int marioWidth, marioHeight, marioNrChannels;
	GLuint textureMarioLeft;
	GLuint textureMarioRight;
	GLuint textureDeadMario;


	GLfloat playerX = 0.0f;

	
	bool jump = false;
	bool canJump = false;
	
	
	//Constructeurs
	Mario();
	//getters et setters
	int getScore() { return score; }
	void setScore() { score += 1; }
	vector<float> getPosition() { return Position; }
	vector<float> getDimension(){ return Dimension; }
	void setLastPosition(bool s) { isLastPositionLeft = s; };
	bool getLastPosition() { return isLastPositionLeft; };
	void  setPosition(vector<float> pos) { Position = pos; }
	
	void UpdatePlayerPosition(vector<platform*>);
	
	void drawPlayerWithTexture(vector<float> pos, vector<float> dim);
	
	void displayScore();
	
	Struct::Collision checkCollision(vector<float> targetPos, vector<float> targetDims,vector<float> platformPos, vector<float> platformDim);
	bool DetectCollisionWPlayer(vector<float> targetPos, vector<float> targetDims);

	Struct::CollisionSide checkEnemyCollision(vector<float> playerPos, vector<float> playerDim, vector<float> platformPos, vector<float> platformDim);
	bool detectGoldCols(vector<float> goldPos, float radius);
	int detectEnnemyCols(vector<float> EnnemyPos, vector<float> dim);
	void loadTexture(int a, const char* fileName);
};
