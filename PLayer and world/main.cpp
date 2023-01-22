

#include "platform.h"
#include "flag.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;

using namespace irrklang;

//texture vars;
unsigned char* marioLeftData;
int marioWidth, marioHeight, marioNrChannels;



string coinSoundstr = "sounds\\coin.ogg";

const char* coinSoundPath = coinSoundstr.c_str();
ISoundEngine* coinSound = createIrrKlangDevice();

GLfloat playerX = 0.0f;
vector<float> gravity = { 0, -0.5 };

vector<float> platformPos = { 0,-1 };
vector<float> platformDim = { 5.0,0.5 };
vector<vector<vector<float>>*> obstacles;

vector<float> platform2Pos = { 5,2 };
vector<float> platform2Dim = { 5.0,0.5 };
vector<float> playerPos = {-1,0};
vector<float> playerDim = {0.5,0.5};


int score = 0;

bool jump;
bool canJump = false;
bool left1;
bool right1;
vector<float> velocity = { 0, 0 };
float jumpForce = 10.0f;
float moveForce = 2.0f;
float deltaTime = 0.0f;
float currentTime = 0.0f;
float lastTime = 0.0f;
float radius = 0.5;
vector<vector<float>> goldPos = { {-5,0},{2, 1} };

float enemyWidth = 0.5f;
vector<vector<float>> enemyPos = { {-5, 0} , {3, 3} };
vector<vector<float>> enemiesSpeed = { {moveForce, 0}, {moveForce,0} };

void drawGold(vector<vector<float>> g) {
    
    for (int j = 0; j < goldPos.size(); j++) {
        glBegin(GL_TRIANGLE_FAN);
        glVertex2f(goldPos[j][0], goldPos[j][1]);
        for (int i = 0; i <= 360; i++) {
            float rad = i * 3.14159 / 180;
            glVertex2f(goldPos[j][0] + cos(rad) * radius, goldPos[j][1] + sin(rad) * radius);
        }
        glEnd();
   }
        
}


GLuint textureMarioLeft;

void loadTexture(const char* filename) {
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
    glGenTextures(1, &textureMarioLeft);
    glBindTexture(GL_TEXTURE_2D, textureMarioLeft);

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

CollisionSide checkEnemyCollision(vector<float> playerPos, vector<float> playerDim, vector<float> platformPos, vector<float> platformDim) {
    
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

void updatePlayerPosition() {
    // Apply gravity
    velocity[1] += gravity[1];

    // Apply jump force if jumping
    if (jump && canJump) {
        velocity[1] = jumpForce;
        jump = false;
        canJump = false;
    }
    if (right1 && left1) {
    
    } else if (right1 ) {
        velocity[0] = moveForce;
    } else if (left1) {
        velocity[0] = -moveForce;
    }
    else {
        velocity[0] = 0;
    }
    // Update player position based on velocity
    playerPos[0] += velocity[0] * deltaTime;
    playerPos[1] += velocity[1] * deltaTime;

    // Check for collision with platform
    //if (DetectCollisionWPlayer(platformPos, platformDim)) {

    //    // Set player position to top of platform
    //    playerPos[1] = platformPos[1] + platformDim[1] + playerDim[1];
    //    velocity[1] = 0;
    //    canJump = true;
    //}
    // Check for collision with platform
    Collision collision = checkCollision(playerPos, playerDim, platformPos, platformDim);
    Collision collision2 = checkCollision(playerPos, playerDim, platform2Pos, platform2Dim);
    vector<Collision> cols = { collision, collision2 };
    for (int i = 0; i < cols.size(); i++) {
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
    }
       

}


void updateEnemiesPosition() {
    for (int j = 0; j < enemyPos.size(); j++) {
        enemiesSpeed[j][1] += gravity[1];


       CollisionSide side2 = checkEnemyCollision(enemyPos[j], { enemyWidth, enemyWidth }, platform2Pos, platform2Dim);
       CollisionSide side = checkEnemyCollision(enemyPos[j], { enemyWidth, enemyWidth }, platformPos, platformDim);


        //moveleft and right for enemies
        if ( side == Left || side2 == Left) {
            enemiesSpeed[j][0] = moveForce;
        }
        if (side == Right || side2 == Right) {

            enemiesSpeed[j][0] = -moveForce;
        }

        // Update player position based on velocity
        enemyPos[j][0] += enemiesSpeed[j][0] * deltaTime;
        enemyPos[j][1] += enemiesSpeed[j][1] * deltaTime;


        Collision collision = checkCollision(enemyPos[j], { enemyWidth , enemyWidth}, platformPos, platformDim);
        Collision collision2 = checkCollision(enemyPos[j], { enemyWidth , enemyWidth }, platform2Pos, platform2Dim);
        vector<Collision> cols = { collision, collision2 };
        for (int i = 0; i < cols.size(); i++)
        {
            if (cols[i].side != None) {
                if (cols[i].side == Top) {
                    enemyPos[j][1] = cols[i].pos[1] + cols[i].dim[1] + enemyWidth;
                    enemiesSpeed[j][1] = 0;
                    
                }
                else if (cols[i].side == Bottom) {
                    enemyPos[j][1] = cols[i].pos[1] - cols[i].dim[1] - enemyWidth;
                    enemiesSpeed[j][1] = 0;
                }
                else if (cols[i].side == Left) {
                    enemyPos[j][0] = cols[i].pos[0] - cols[i].dim[0] - enemyWidth;
                    enemiesSpeed[j][0] = 0;
                }
                else if (cols[i].side == Right) {
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

void drawPlayerWithTexture(vector<float> pos, vector<float> dim) {

    glColor3f(1.0f, 1.0f, 1.0f);
    // Enable texture coordinates
    glEnable(GL_TEXTURE_2D);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, textureMarioLeft);

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

void timer(int value) {
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    updatePlayerPosition();
    updateEnemiesPosition();
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}


void detectGoldCols() {
    for (int i = 0; i < goldPos.size(); i++) {
        Collision collision = checkCollision(playerPos, playerDim, goldPos[i], { radius, radius });
        if (collision.side != None) {
            goldPos.erase(goldPos.begin() + i);
            score += 1;
            coinSound->play2D(coinSoundPath, false);
            cout << "your score : " << score << endl;
            return;
            }
        }
    }

void displayScore() {
    std::string score_str = std::to_string(score);
    glRasterPos2f(0, 18);
    for (int i = 0; i < score_str.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_str[i]);
    }
}

void display()
{
    
    updatePlayerPosition();
    updateEnemiesPosition();

   

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


    // Draw the player square
    drawPlayerWithTexture(playerPos, playerDim);



    glColor3f(1.0, 0.843137, 0.0);
    drawGold(goldPos);

    detectGoldCols();
 
    glColor3f(1.0f, 1.0f, 0.0f);
    //drawRectangle(platformPos, platformDim);
    //drawRectangle(platform2Pos, platform2Dim);
    platform* plat1=new platform(platformPos, platformDim);
    platform* plat2 = new platform(platform2Pos, platform2Dim);
    plat1->draw();
    plat2->draw();
    
    //draw flag
    vector<float> flagPos = { 2,0 };
    vector<float> flagDim = { 0.5,0.5 };
    flag* flag1 = new flag(flagPos, flagDim);
    flag1->drawWithTexture("flag.png");


    glColor3f(1.0f, 0.0f, 0.0f);

    for (int i = 0; i < enemyPos.size(); i++) {
        drawRectangle(enemyPos[i], { enemyWidth, enemyWidth });
    }

    glColor3f(1.0f, 1.0f, 1.0f);
    displayScore();


    glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
    switch (key)  {
    case 'q':
       left1 = true;
        break;
    case 'd':
        right1 = true;
        break;
    case 'z':
        jump = true;
        break;
    }
    glutPostRedisplay();
}

void keyboardUp(unsigned char key, int x, int y) {
    switch (key) {
    case 'q':
        left1 = false;
        
        break;
    case 'd':
        right1 = false;
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
    ISoundEngine* SoundEngine = createIrrKlangDevice();
   // SoundEngine->play2D(themeSong, true);
    
    //const char* c= &music;
    //SoundEngine->play2D(s, false);
  
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Player and World");
    glClearColor(0.49, 0.47, 0.87, 1.0f);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    loadTexture("marioLeft.jpg");
    glutDisplayFunc(display);

    glutTimerFunc(1000 / 60, timer, 0);
    glutMainLoop();

    return 0;
}
