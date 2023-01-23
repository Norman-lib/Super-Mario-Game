#pragma once

#include "utils.h"
#include "platform.h"
/// <summary>
/// cette classe permet de définir l'objet ennenmy qui représente un danger pour Mario
/// </summary>
class Enemy {
private:
	/// <summary>
	/// cette classe contient 9 attributs
	/// position : vecteur de position
	/// dimentions : la taille de l'objet dessiné
	/// speed : la vitesse de l'ennemi
	/// deltaTime
	/// moveForce : la force du mouvement 
	/// texturePath : un pointeur vers le nom du fichier texture 
	/// texture : un GLuint contenant les données de la texture
	/// plateformPos : la position de la plateforme 
	/// plateformDim : les dimensions de la plateforme
	/// </summary>
	vector<float> position;
	vector <float> dimentions;
	vector<float> speed;
	float deltaTime;
	float moveForce;
	const char * texturePath;
	GLuint texture;
	vector<float> platformPos;
	vector <float> platformDim;

public:
	//Constructeurs
	Enemy(vector<float> pos, vector<float> dim, vector<float> posP, vector<float> dimP);
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
	//methodes
	void drawEnemy();
	void Move();
	void loadTexture();



};

