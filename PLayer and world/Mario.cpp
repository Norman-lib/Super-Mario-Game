#include"Mario.h"


Mario::Mario()
{
    textureRight = true;
}

void Mario::UpdatePlayerPosition(vector<float> platformPos, vector<float> platformDim)//vector of platforms.....................
{
    // Apply gravity
    velocity[1] += gravity[1];

    // Apply jump force if jumping
    if (jump && canJump) {
        velocity[1] = jumpForce;
        jump = false;
        canJump = false;
    }
    if (right1 && left1) {

    }
    else if (right1) {
        velocity[0] = moveForce;
    }
    else if (left1) {
        velocity[0] = -moveForce;
    }
    else {
        velocity[0] = 0;
    }

    // Update player position based on velocity
    Position[0] += velocity[0] * deltaTime;
    Position[1] += velocity[1] * deltaTime;

    // Check for collision with platform
    Struct::Collision collision = checkCollision(Position,Dimension,platformPos, platformDim);
    
    Struct::Collision collision2 = checkCollision(Position, Dimension, platformPos, platformDim); //To be changed 2 for the second platform ...................................
    vector<Struct::Collision> cols = { collision, collision2 };
    for (int i = 0; i < cols.size(); i++) {
        if (cols[i].side != Struct::None) {
            if (cols[i].side == Struct::Top) {
                Position[1] = cols[i].pos[1] + cols[i].dim[1] + Dimension[1];
                velocity[1] = 0;
                canJump = true;
            }
            else if (cols[i].side == Struct::Bottom) {
                Position[1] = cols[i].pos[1] - cols[i].dim[1] - Dimension[1];
                velocity[1] = 0;
            }
            else if (cols[i].side == Struct::Left) {
                Position[0] = cols[i].pos[0] - cols[i].dim[0] - Dimension[0];
                velocity[0] = 0;
            }
            else if (cols[i].side == Struct::Right) {
                Position[0] = cols[i].pos[0] + cols[i].dim[0] + Dimension[0];
                velocity[0] = 0;
            }
        }
    }
}

void  Mario::drawPlayerWithTexture(vector<float> pos, vector<float> dim) {

    glColor3f(1.0f, 1.0f, 1.0f);
    // Enable texture coordinates
    glEnable(GL_TEXTURE_2D);

    // Bind the texture
     
        
        if (left1) {
            glBindTexture(GL_TEXTURE_2D, textureMarioLeft);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, textureMarioRight);
        }
    
    

    // Draw the shape
    glBegin(GL_QUADS);
    // Set the texture coordinates for each vertex
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(pos[0] + dim[0], pos[1] + dim[1]);
    glTexCoord2f(1.0f, 0.0f);
    glVertex2f(pos[0] - dim[0], pos[1] + dim[1]);
    glTexCoord2f(1.0f, 1.0f);
    glVertex2f(pos[0] - dim[0], pos[1] - dim[1]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(pos[0] + dim[0], pos[1] - dim[1]);
    glEnd();

    // Unbind the texture
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Mario::displayScore()
{
    std::string score_str = std::to_string(score);
    glRasterPos2f(0, 18);
    for (int i = 0; i < score_str.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_str[i]);
    }
}

Struct::Collision Mario::checkCollision(vector<float>PlayerPos, vector<float> PlayerDim, vector<float> platformPos, vector<float> platformDim)
{
    // Calculate the edges of the player square
    Struct::Collision col;
    col.dim = platformDim;
    col.pos = platformPos;
    float playerLeft = PlayerPos[0] - PlayerDim[0];
    float playerRight = PlayerPos[0] + PlayerDim[0];
    float playerTop = PlayerPos[1] + PlayerDim[1];
    float playerBottom = PlayerPos[1] - PlayerDim[1];

    // Calculate the edges of the platform rectangle
    float platformLeft = platformPos[0] - platformDim[0];
    float platformRight = platformPos[0] + platformDim[0];
    float platformTop = platformPos[1] + platformDim[1];
    float platformBottom = platformPos[1] - platformDim[1];

    // Check for collision on the top side
    if (playerBottom < platformTop && playerTop > platformTop &&
        playerRight > platformLeft && playerLeft < platformRight) {
        col.side = Struct::Top;
        return col;
    }

    // Check for collision on the bottom side
    if (playerTop > platformBottom && playerBottom < platformBottom &&
        playerRight > platformLeft && playerLeft < platformRight) {
        col.side = Struct::Bottom;
        return col;
    }

    // Check for collision on the left side
    if (playerRight > platformLeft && playerLeft < platformLeft &&
        playerBottom < platformTop && playerTop > platformBottom) {
        col.side = Struct:: Left;
        return col;

    }

    // Check for collision on the right side
    if (playerLeft < platformRight && playerRight > platformRight &&
        playerBottom < platformTop && playerTop > platformBottom) {
        col.side = Struct:: Right;
        return col;
    }

    // If no collision is detected, return None
    col.side = Struct:: None;
    return col;
}

bool Mario::DetectCollisionWPlayer(vector<float> targetPos, vector<float> targetDims)
{
    //chack these one ...............
    return Position[1] - Dimension[1] < targetPos[1] + targetDims[1] &&
        Position[0] + Dimension[0] > targetPos[0] - targetDims[0] &&
        Position[0] - Dimension[0] < targetPos[0] + targetDims[0] &&
        Position[0] + Dimension[0] > targetPos[0] - targetDims[0];
}

Struct::CollisionSide Mario::checkEnemyCollision(vector<float> playerPos, vector<float> playerDim, vector<float> platformPos, vector<float> platformDim) {

    float playerLeft = Position[0] - Dimension[0];
    float playerRight = Position[0] + Dimension[0];
    float playerTop = Position[1] + Dimension[1];
    float playerBottom = Position[1] - Dimension[1];

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
        return Struct::Left;

    }

    // Check for collision on the right top side
    if (playerLeft < platformRight && playerRight > platformRight
        && abs(playerBottom - platformTop) < 0.1

        //top
       /* playerBottom < platformTop && playerTop > platformTop &&
        playerRight > platformLeft && playerLeft < platformRight*/
        ) {

        return Struct::Right;
    }
    // if nothing return None
    return Struct::None;
}

void Mario::detectGoldCols(vector<vector<float>> goldPos,float radius) {
    for (int i = 0; i < goldPos.size(); i++) {
        Struct::Collision collision = checkCollision( Position,Dimension,goldPos[i], { radius, radius });
        if (collision.side != Struct::None) {
            goldPos.erase(goldPos.begin() + i);
            score += 1;
            //coinSound->play2D(coinSoundPath, false);
            cout << "your score : " << score << endl;
            return;
        }
    }
}


void Mario::detectEnnemyCols(vector<vector<float>> EnnemyPos, vector<float> dim) {
    for (int i = 0; i < EnnemyPos.size(); i++) {
        Struct::Collision collision = checkCollision(Position, Dimension, EnnemyPos[i], { dim[0],dim[1] });
        if (collision.side != Struct::None) {
            EnnemyPos.erase(EnnemyPos.begin() + i);

            cout << "Game Over";/////
            return;
        }
    }
}