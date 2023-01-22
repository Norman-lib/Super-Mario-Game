


#include "platform.h"
#include "flag.h"

#include "Gold.h" 

#include"Mario.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Ennemy.h"




vector<Gold*> Golds ; // � remplir avec le gold 


using namespace std;



//texture vars;
unsigned char* marioLeftData;
int marioWidth, marioHeight, marioNrChannels;

Mario mario = Mario();


    string coinSoundstr = "sounds\\coin.ogg";


const char* coinSoundPath = coinSoundstr.c_str();
//ISoundEngine* coinSound = createIrrKlangDevice();


GLfloat playerX = 0.0f;
vector<float> gravity = { 0, -0.5 };

//platform
vector<float> platformPos = { 0,-1 };
vector<float> platformDim = { 10.0,0.5 };
vector<float> platform2Pos = { 5,2.0 };
vector<float> platform2Dim = { 5.0,0.5 };
vector<float> platform3Pos = { 0.0,5.0 };
vector<float> platform3Dim = { 3.0,0.5 };
vector<float> platform4Pos = { -3.0,8.0 };
vector<float> platform4Dim = { 7.0,0.5 };
vector<float> platform5Pos = { 9.0,6.0 };
vector<float> platform5Dim = { 3.0,0.5 };


//flag
vector<float> flagPos = { -9.5,9.0 };
vector<float> flagDim = { 0.5,0.5 };

vector<vector<vector<float>>*> obstacles;

vector<float> playerPos = {-1,0};
vector<float> playerDim = {0.5,0.5};

platform* plat1 = new platform(platformPos, platformDim);
platform* plat2 = new platform(platform2Pos, platform2Dim); 
platform* plat3 = new platform(platform3Pos, platform3Dim);
platform* plat4 = new platform(platform4Pos, platform4Dim);
platform* plat5 = new platform(platform5Pos, platform5Dim);

Gold* gold1 = new Gold();
Gold* gold2 = new Gold();
Gold* gold3 = new Gold();
Gold* gold4 = new Gold();

flag* flag1 = new flag(flagPos, flagDim);

vector<platform*> platformList;

int score = 0;


float deltaTime = 0.0f;
float currentTime = 0.0f;
float lastTime = 0.0f;
float radius = 0.5;
// vector<vector<float>> goldPos = { {-5,0},{2, 1} };

float enemyWidth = 0.5f;
float moveForce = 2.0f;
vector<vector<float>> enemyPos = {   {3, 3} };
vector<vector<float>> enemiesSpeed = {{moveForce, 0} };


Enemy* enemy = new Enemy({ 5, 0 }, { enemyWidth, enemyWidth }, plat1->getPos(), plat1->getDim());
Enemy* enemy1 = new Enemy({ 5, 3 }, { enemyWidth, enemyWidth }, plat2->getPos(), plat2->getDim());

vector <Enemy*> enemies = { enemy, enemy1 };




void loadTextureLeft(const char* filename) {
    int width, height, numComponents;
    unsigned char* imageData = stbi_load(filename, &width, &height, &numComponents, 0);

    if (imageData == NULL)
        std::cerr << "Unable to load texture: " << filename << std::endl;
    else {
        cout<<"worked" << endl;
        cout << "width :" << width << endl;
        cout << "width :" << height << endl;
        cout << "width :" << numComponents << endl;

    }
   
    // Generate the texture object
    glGenTextures(1, &mario.textureMarioLeft);
    glBindTexture(GL_TEXTURE_2D, mario.textureMarioLeft);

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

void loadTextureRight(const char* filename) {
    int width, height, numComponents;
    unsigned char* imageData = stbi_load(filename, &width, &height, &numComponents, 0);

    if (imageData == NULL)
        std::cerr << "Unable to load texture: " << filename << std::endl;
    else {
        cout << "worked" << endl;
        cout << "width :" << width << endl;
        cout << "width :" << height << endl;
        cout << "width :" << numComponents << endl;

    }
    // Generate the texture object
    glGenTextures(1, &mario.textureMarioRight);
    glBindTexture(GL_TEXTURE_2D, mario.textureMarioRight);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Copy the image data to the texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Free the image data
    stbi_image_free(imageData);
}



void drawRectangle(vector<float> pos, vector<float> dim ) {
    glBegin(GL_QUADS);
    glVertex2f(pos[0] - dim[0], pos[1] - dim[1]);
    glVertex2f(pos[0] - dim[0], pos[1] + dim[1]);
    glVertex2f(pos[0] + dim[0], pos[1] + dim[1]);
    glVertex2f(pos[0] + dim[0], pos[1] - dim[1]);
    glEnd();
}


enum CollisionSide {
    None,
    Top,
    Bottom,
    Left,
    Right,
};
struct Collision {
    CollisionSide side;
    vector<float> pos;
    vector<float> dim;
};


Collision checkCollision(vector<float> playerPos, vector<float> playerDim, vector<float> platformPos, vector<float> platformDim) {
        // Calculate the edges of the player square
        Collision col;
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

        // Check for collision on the bottom side
        if (playerTop > platformBottom && playerBottom < platformBottom &&
            playerRight > platformLeft && playerLeft < platformRight) {
            col.side = Bottom;
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

bool DetectCollisionWPlayer(vector<float> targetPos, vector<float> targetDims) {
    return playerPos[1] - playerDim[1] < targetPos[1] + targetDims[1] &&
        playerPos[0] + playerDim[0] > targetPos[0] - targetDims[0] &&
        playerPos[0] - playerDim[0] < targetPos[0] + targetDims[0] &&
        playerPos[0] + playerDim[0] > targetPos[0] - targetDims[0];
}

CollisionSide checkEnemyCollision1(vector<float> playerPos, vector<float> playerDim, vector<float> platformPos, vector<float> platformDim) {
    
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
        && abs(playerBottom - platformTop) <0.1
        
        //top
       /* playerBottom < platformTop && playerTop > platformTop &&
        playerRight > platformLeft && playerLeft < platformRight*/
        ) {
       
        return Right;
    }
    // if nothing return None
    return None;
}

//void updatePlayerPosition() {
//    // Apply gravity
//    velocity[1] += gravity[1];
//
//    // Apply jump force if jumping
//    if (jump && canJump) {
//        velocity[1] = jumpForce;
//        jump = false;
//        canJump = false;
//    }
//    if (right1 && left1) {
//    
//    } else if (right1) {
//        velocity[0] = moveForce;
//    } else if (left1) {
//        velocity[0] = -moveForce;
//    }
//    else {
//        velocity[0] = 0;
//    }
//    // Update player position based on velocity
//    playerPos[0] += velocity[0] * deltaTime;
//    playerPos[1] += velocity[1] * deltaTime;
//
//    // Check for collision with platform
//    //if (DetectCollisionWPlayer(platformPos, platformDim)) {
//
//    //    // Set player position to top of platform
//    //    playerPos[1] = platformPos[1] + platformDim[1] + playerDim[1];
//    //    velocity[1] = 0;
//    //    canJump = true;
//    //}
//    // Check for collision with platform
//    Collision collision = checkCollision(playerPos, playerDim, platformPos, platformDim);
//    Collision collision2 = checkCollision(playerPos, playerDim, platform2Pos, platform2Dim);
//    vector<Collision> cols = { collision, collision2 };
//    for (int i = 0; i < cols.size(); i++) {
//        if (cols[i].side != None) {
//            if (cols[i].side == Top) {
//                playerPos[1] = cols[i].pos[1] + cols[i].dim[1] + playerDim[1];
//                velocity[1] = 0;
//                canJump = true;
//            }
//            else if (cols[i].side == Bottom) {
//                playerPos[1] = cols[i].pos[1] - cols[i].dim[1] - playerDim[1];
//                velocity[1] = 0;
//            }
//            else if (cols[i].side == Left) {
//                playerPos[0] = cols[i].pos[0] - cols[i].dim[0] - playerDim[0];
//                velocity[0] = 0;
//            }
//            else if (cols[i].side == Right) {
//                playerPos[0] = cols[i].pos[0] + cols[i].dim[0] + playerDim[0];
//                velocity[0] = 0;
//            }
//        }
//    }
//       
//
//}


void updateEnemiesPosition(vector<float> pos, vector<float> dim) {
    for (int j = 0; j < enemyPos.size(); j++) {
        enemiesSpeed[j][1] += gravity[1];


        Struct::CollisionSide side2 = mario.checkEnemyCollision(enemyPos[j], { enemyWidth, enemyWidth }, pos, dim);
        Struct::CollisionSide side = mario.checkEnemyCollision(enemyPos[j], { enemyWidth, enemyWidth }, platformPos, platformDim);


        //moveleft and right for enemies
        if ( side == Struct::CollisionSide::Left || side2 == Struct::CollisionSide::Left) {
            enemiesSpeed[j][0] = mario.moveForce;
        }
        if (side == Struct::CollisionSide::Right || side2 == Struct::CollisionSide::Right) {

            enemiesSpeed[j][0] = -mario.moveForce;
        }

        // Update player position based on velocity
        enemyPos[j][0] += enemiesSpeed[j][0] * deltaTime;
        enemyPos[j][1] += enemiesSpeed[j][1] * deltaTime;


        Struct::Collision collision = mario.checkCollision(enemyPos[j], { enemyWidth , enemyWidth}, platformPos, platformDim);
        Struct::Collision collision2 = mario.checkCollision(enemyPos[j], { enemyWidth , enemyWidth }, platform2Pos, platform2Dim);
        vector<Struct::Collision> cols = { collision, collision2 };
        for (int i = 0; i < cols.size(); i++)
        {
            if (cols[i].side != Struct::CollisionSide::None) {
                if (cols[i].side == Struct::CollisionSide::Top) {
                    enemyPos[j][1] = cols[i].pos[1] + cols[i].dim[1] + enemyWidth;
                    enemiesSpeed[j][1] = 0;
                    
                }
                else if (cols[i].side == Struct::CollisionSide::Bottom) {
                    enemyPos[j][1] = cols[i].pos[1] - cols[i].dim[1] - enemyWidth;
                    enemiesSpeed[j][1] = 0;
                }
                else if (cols[i].side == Struct::CollisionSide::Left) {
                    enemyPos[j][0] = cols[i].pos[0] - cols[i].dim[0] - enemyWidth;
                    enemiesSpeed[j][0] = 0;
                }
                else if (cols[i].side == Struct::CollisionSide::Right) {
                    enemyPos[j][0] = cols[i].pos[0] + cols[i].dim[0] + enemyWidth;
                    enemiesSpeed[j][0] = 0;
                }
            }
        }


    }



    // Apply gravity
    //velocity[1] += gravity[1];


   
   
     /*if (right1) {
        velocity[0] = moveForce;
    }
    else if (left1) {
        velocity[0] = -moveForce;
    }
    else {
        velocity[0] = 0;
    }*/
    // Update player position based on velocity
   /* playerPos[0] += velocity[0] * deltaTime;
    playerPos[1] += velocity[1] * deltaTime;*/

    // Check for collision with platform
    //if (DetectCollisionWPlayer(platformPos, platformDim)) {

    //    // Set player position to top of platform
    //    playerPos[1] = platformPos[1] + platformDim[1] + playerDim[1];
    //    velocity[1] = 0;
    //    canJump = true;
    //}
    // Check for collision with platform
   /* Collision collision = checkCollision(playerPos, playerDim, platformPos, platformDim);
    Collision collision2 = checkCollision(playerPos, playerDim, platform2Pos, platform2Dim);
    vector<Collision> cols = { collision, collision2 };
    for (int i = 0; i < cols.size(); i++) 
    {
        if (cols[i].side != None) {
            if (cols[i].side == Top) {
                playerPos[1] = cols[i].pos[1] + cols[i].dim[1] + playerDim[1];
                velocity[1] = 0;
                canJump = true;
            }
            else if (cols[i].side == Bottom) {
                playerPos[1] = cols[i].pos[1] - cols[i].dim[1] - playerDim[1];
                velocity[1] = 0;
            }
            else if (cols[i].side == Left) {
                playerPos[0] = cols[i].pos[0] - cols[i].dim[0] - playerDim[0];
                velocity[0] = 0;
            }
            else if (cols[i].side == Right) {
                playerPos[0] = cols[i].pos[0] + cols[i].dim[0] + playerDim[0];
                velocity[0] = 0;
            }
        }
    }*/


}



void timer(int value) {
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentTime - lastTime) / 1000.0f;
    
    
    lastTime = currentTime;
   // updatePlayerPosition();
    mario.currentTime = currentTime;
    mario.deltaTime = deltaTime;
    mario.lastTime = currentTime;
    mario.UpdatePlayerPosition(platformPos,platformDim);
   
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i]->setDeltaTime(deltaTime);
        enemies[i]->Move();
    }
   
    glutPostRedisplay();
   
    glutTimerFunc(1000 / 60, timer, 0);
}


void detectGoldCols(vector<float> playerPos, vector<float> playerDim, int score, Gold* gold) { //getScore au lieu de score
   
        Collision collision = checkCollision(playerPos, playerDim, gold->getPosition(), { radius, radius });
        if (collision.side != None) {

            Golds.erase(find(Golds.begin(), Golds.end(), gold)); 
            score += 1; //SetScore() du player
            //coinSound->play2D(coinSoundPath, false);

            cout << "your score : " << score << endl;
            return;
        }
    
  
}








int colValue;

void display()
{
    mario.UpdatePlayerPosition(platformPos,platformDim);
    
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i]->Move();
    }



    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-20.0, 20.0, -20.0, 20.0, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Draw the world rectangle
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex2f(-18.0f, 18.0f);
    glVertex2f(18.0f, 18.0f);
    glVertex2f(18.0f, -18.0f);
    glVertex2f(-18.0f, -18.0f);
    glEnd();
    glColor3f(0.0, 1.0, 0.0);
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i]->drawEnemy();
    }

    // Draw the player square
    mario.drawPlayerWithTexture(mario.getPosition(), mario.getDimension());



    glColor3f(1.0, 0.843137, 0.0);
    for (int i = 0; i < Golds.size(); i++) {
        Golds[i]->drawGold(Golds[i]->getPosition());
        if (mario.detectGoldCols(Golds[i]->getPosition(), radius)) {
            Golds.erase(Golds.begin() + i);
         }
        
    }

   
    for (int i = 0; i < enemies.size(); i++) {
        colValue = mario.detectEnnemyCols(enemies[i]->getPosition(), enemies[i]->getDimentions());
        if (colValue == 1) {
            enemies.erase(enemies.begin() + i);
            colValue = 0;
        }
        else if (colValue == 2) {
            cout << "Kill MARIO" << endl;
        }
    }
    
    glColor3f(1.0f, 1.0f, 0.0f);
    //drawRectangle(platformPos, platformDim);
    //drawRectangle(platform2Pos, platform2Dim);

    for (int i = 0; i < platformList.size(); i++) {
        platformList[i]->draw();
    }



    //draw flag

 
    flag1->drawWithTexture();


    glColor3f(1.0f, 0.0f, 0.0f);

    for (int i = 0; i < enemyPos.size(); i++) {
        drawRectangle(enemyPos[i], { enemyWidth, enemyWidth });
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    mario.displayScore();


    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
    case 'q':

        mario.left1 = true;
        mario.setLastPosition(true);


        break;
    case 'd':
        mario.right1 = true;
        mario.setLastPosition(false);
       

        break;
    case 'z':
        mario.jump = true;
        break;
    }
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        mario.left1 = false;
        
        break;
    case 'd':
        mario.right1 = false;
        cout << mario.right1;
        
        break;
    case 'z':
        
        break;
    }
    glutPostRedisplay();
}




int main(int argc, char** argv)
{
    string themeSongstr = "C:\\Users\\Norma\\source\\repos\\Piano-freeGlut-\\Piano (freeGlut)\\piano\\black notes\\100.ogg";
    themeSongstr = "sounds\\theme.ogg";
    const char* themeSong = themeSongstr.c_str();

   // SoundEngine->play2D(themeSong, true);
    
    //const char* c= &music;
    //SoundEngine->play2D(s, false);
    platformList.push_back(plat1);
    platformList.push_back(plat2);
    platformList.push_back(plat3);
    platformList.push_back(plat4);
    platformList.push_back(plat5);


    gold1->setPosition({ -5,0 });
    gold2->setPosition({ 2, 1 });
    gold3->setPosition({ -7, 0 });
    gold4->setPosition({ 4, 1 });
   

    Golds.push_back(gold1); 
    Golds.push_back(gold2); 
    Golds.push_back(gold3);
    Golds.push_back(gold4);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Player and World");
    glClearColor(0.49, 0.47, 0.87, 1.0f);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    
    for (int i = 0; i < Golds.size(); i++) {
        Golds[i]->loadTexture("coin.jpg");
    }
    for (int i = 0; i < enemies.size(); i++) {
        enemies[i]->loadTexture();
    }
    for (int i = 0; i < platformList.size(); i++) {
        platformList[i]->loadTexture();
    }
    
    flag1->loadTexture();
    mario.loadTexture(0, "marioLeft.jpg");
    mario.loadTexture(1, "marioRight.jpg");
    mario.loadTexture(2, "dead.jpg");

   /* loadTextureLeft("marioLeft.jpg");
    loadTextureRight("marioRight.jpg");*/
    glutDisplayFunc(display);

    glutTimerFunc(1000 / 60, timer, 0);
    glutMainLoop();

    return 0;
}
