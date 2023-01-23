#include "Ennemy.h"

#include "stb_image.h"



Enemy::Enemy(vector<float> pos, vector<float> dim, vector<float> posP, vector<float> dimP) {

    position = pos;
    dimentions = dim;
    platformDim = dimP;
    platformPos = posP;
    moveForce = 2.0;
    deltaTime = 0;
    speed = { moveForce,0 };
    texturePath = "gomba.jpg";
}



void Enemy :: drawEnemy() {
    glColor3f(1.0, 1.0, 1.0);
    glEnable(GL_TEXTURE_2D);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(position[0] + dimentions[0], position[1] + dimentions[1]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(position[0] - dimentions[0], position[1] + dimentions[1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(position[0] - dimentions[0], position[1] - dimentions[1]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(position[0] + dimentions[0], position[1] - dimentions[1]);
        glEnd();
    
}
enum CollisionSideEnemies {
    None,
    Top,
    Left,
    Right,
};
struct Collision1 {
    CollisionSideEnemies side;
    vector<float> pos;
    vector<float> dim;

};

CollisionSideEnemies checkEnemyCollision1(vector<float> playerPos, vector<float> playerDim, vector<float> platformPos, vector<float> platformDim) {

    float playerLeft = playerPos[0] - playerDim[0];
    float playerRight = playerPos[0] + playerDim[0];
    float playerTop = playerPos[1] + playerDim[1];
    float playerBottom = playerPos[1] - playerDim[1];

    // Calculate the edges of the platform rectangle
    float platformLeft = platformPos[0] - platformDim[0];
    float platformRight = platformPos[0] + platformDim[0];
    float platformTop = platformPos[1] + platformDim[1];
    float platformBottom = platformPos[1] - platformDim[1];

    // Check for collision on the left top side
    if (playerRight > platformLeft && playerLeft < platformLeft
        && abs(playerBottom - platformTop) < 0.1
        //left
         //top
        ) {
        return Left;

    }

    // Check for collision on the right top side
    if (playerLeft < platformRight && playerRight > platformRight
        && abs(playerBottom - platformTop) < 0.1

        //top
       /* playerBottom < platformTop && playerTop > platformTop &&
        playerRight > platformLeft && playerLeft < platformRight*/
        ) {

        return Right;
    }
    // if nothing return None
    return None;
}

Collision1 checkCollision(vector<float> playerPos, vector<float> playerDim, vector<float> platformPos, vector<float> platformDim) {
    // Calculate the edges of the player square
    Collision1 col;
    col.dim = platformDim;
    col.pos = platformPos;
    float playerLeft = playerPos[0] - playerDim[0];
    float playerRight = playerPos[0] + playerDim[0];
    float playerTop = playerPos[1] + playerDim[1];
    float playerBottom = playerPos[1] - playerDim[1];

    // Calculate the edges of the platform rectangle
    float platformLeft = platformPos[0] - platformDim[0];
    float platformRight = platformPos[0] + platformDim[0];
    float platformTop = platformPos[1] + platformDim[1];
    float platformBottom = platformPos[1] - platformDim[1];

    // Check for collision on the top side
    if (playerBottom < platformTop && playerTop > platformTop &&
        playerRight > platformLeft && playerLeft < platformRight) {
        col.side = Top;
        return col;
    }


    // Check for collision on the left side
    if (playerRight > platformLeft && playerLeft < platformLeft &&
        playerBottom < platformTop && playerTop > platformBottom) {
        col.side = Left;
        return col;

    }

    // Check for collision on the right side
    if (playerLeft < platformRight && playerRight > platformRight &&
        playerBottom < platformTop && playerTop > platformBottom) {
        col.side = Right;
        return col;
    }





    // If no collision is detected, return None
    col.side = None;
    return col;
}

void Enemy::Move() {
 
      


        CollisionSideEnemies side2 = checkEnemyCollision1(position, dimentions, platformPos, platformDim);
        


        //moveleft and right for enemies
        if ( side2 == Left) {
            speed[0] = moveForce;
        }
        if ( side2 == Right) {

            speed[0] = -moveForce;
        }

        // Update player position based on velocity
        position[0] += speed[0] * deltaTime;
      

}

void Enemy::loadTexture() {
   
        int width, height, numComponents;
        unsigned char* imageData = stbi_load(texturePath, &width, &height, &numComponents, 0);

        if (imageData == NULL)
            std::cerr << "Unable to load texture: " << texturePath << std::endl;
        else {
            cout << "worked" << endl;
            cout << "enemy :" << width << endl;
            cout << "enemy :" << height << endl;
            cout << "enemy :" << numComponents << endl;

        }
        // Generate the texture object
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Copy the image data to the texture object
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // Free the image data
        stbi_image_free(imageData);
    
}