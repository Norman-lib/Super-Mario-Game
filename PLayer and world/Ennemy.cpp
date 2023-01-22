#include "Ennemy.h"


Enemy::Enemy(vector<float> pos, vector<float> dim) {
    position = pos;
    dimentions = dim;
}

void Enemy :: drawEnemy() {
        glBegin(GL_QUADS);
        glVertex2f(position[0] - dimentions[0], position[1] - dimentions[1]);
        glVertex2f(position[0] - dimentions[0], position[1] + dimentions[1]);
        glVertex2f(position[0] + dimentions[0], position[1] + dimentions[1]);
        glVertex2f(position[0] + dimentions[0], position[1] - dimentions[1]);
        glEnd();
    
}

void Enemy::Move() {

}