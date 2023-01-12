
#include <gl/glew.h>
#include <GL/freeglut.h>
#include <vector>
#include <string>;
#include <iostream>
#include <irrKlang.h>

using namespace std;

using namespace irrklang;


string coinSoundstr = "sounds\\coin.ogg";

const char* coinSoundPath = coinSoundstr.c_str();
ISoundEngine* coinSound = createIrrKlangDevice();

GLfloat playerX = 0.0f;
vector<float> gravity = { 0, -0.5 };

vector<float> platformPos = { 0,-1 };
vector<float> platformDim = { 10.0,0.5 };
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
void timer(int value) {
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = (currentTime - lastTime) / 1000.0f;
    lastTime = currentTime;
    updatePlayerPosition();
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
    glColor3f(0.0f, 1.0f, 0.0f);
    drawRectangle(playerPos, playerDim);

    glColor3f(1.0, 0.843137, 0.0);
    drawGold(goldPos);

    detectGoldCols();
 
    glColor3f(1.0f, 1.0f, 0.0f);
    drawRectangle(platformPos, platformDim);
    drawRectangle(platform2Pos, platform2Dim);

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
    SoundEngine->play2D(themeSong, true);
    
    //const char* c= &music;
    //SoundEngine->play2D(s, false);
  
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Player and World");
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);
    glutDisplayFunc(display);

    glutTimerFunc(1000 / 60, timer, 0);
    glutMainLoop();

    return 0;
}
