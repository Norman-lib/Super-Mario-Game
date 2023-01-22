#include"Mario.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Mario mario = Mario();


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



int score = 0;


float deltaTime = 0.0f;
float currentTime = 0.0f;
float lastTime = 0.0f;
float radius = 0.5;
vector<vector<float>> goldPos = { {-5,0},{2, 1} };

float enemyWidth = 0.5f;
vector<vector<float>> enemyPos = { {-5, 0} , {3, 3} };
vector<vector<float>> enemiesSpeed = { {mario.moveForce, 0}, {mario.moveForce,0} };



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



void updateEnemiesPosition()
{
    for (int j = 0; j < enemyPos.size(); j++) {
        enemiesSpeed[j][1] += gravity[1];


        Struct::CollisionSide side2 = mario.checkEnemyCollision(enemyPos[j], { enemyWidth, enemyWidth }, platform2Pos, platform2Dim);
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
    mario.currentTime = glutGet(GLUT_ELAPSED_TIME);
    mario.deltaTime = (mario.currentTime - mario.lastTime) / 1000.0f;
    mario.lastTime = mario.currentTime;
    mario.UpdatePlayerPosition(platformPos,platformDim);
    updateEnemiesPosition();
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}





void display()
{
    mario.UpdatePlayerPosition(platformPos,platformDim);
    
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
    mario.drawPlayerWithTexture(mario.getPosition(), mario.getDimension());

    glColor3f(1.0, 0.843137, 0.0);
    drawGold(goldPos);

    mario.detectGoldCols(goldPos,radius);
    mario.detectEnnemyCols(enemyPos, { enemyWidth,enemyWidth });
    glColor3f(1.0f, 1.0f, 0.0f);
    drawRectangle(platformPos, platformDim);
    drawRectangle(platform2Pos, platform2Dim);



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
    switch (key)  {
    case 'q':
       
        mario.left1 = true;     
        mario.textureMarioRight = true;

       
        break;
    case 'd':
        mario.right1 = true;
        mario.textureMarioRight = false;
        
        break;
    case 'z':
        mario.jump = true;
        break;
    }
    glutPostRedisplay();
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
    loadTextureLeft("marioLeft.jpg");
    loadTextureRight("marioRight.jpg");
    glutDisplayFunc(display);

    glutTimerFunc(1000 / 60, timer, 0);
    glutMainLoop();

    return 0;
}
