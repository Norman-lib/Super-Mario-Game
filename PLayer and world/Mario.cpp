#include"Mario.h"
#include "stb_image.h"

/// <summary>
/// Contructeur de Mario (joueur) 
/// </summary>
Mario::Mario()
{
    gameOver = false;
    isLastPositionLeft = false;
    
}
/// <summary>
/// Modifier la position du joueur 
/// </summary>
/// <param name="Platform">Vecteur de platformes du jeu</param>
void Mario::UpdatePlayerPosition(vector<platform*> plats)
{

    if (!gameOver) {
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
       

        
        for (int i = 0; i < plats.size(); i++) {
            Struct::Collision collision = checkCollision(Position, Dimension, plats[i]->getPos(), plats[i]->getDim());
            if (collision.side != Struct::None) {
                if (collision.side == Struct::Top) {
                    Position[1] = collision.pos[1] + collision.dim[1] + Dimension[1];
                    velocity[1] = 0;
                    canJump = true;
                }
                else if (collision.side == Struct::Bottom) {
                    Position[1] = collision.pos[1] - collision.dim[1] - Dimension[1];
                    velocity[1] = 0;
                }
                else if (collision.side == Struct::Left) {
                    Position[0] = collision.pos[0] - collision.dim[0] - Dimension[0];
                    velocity[0] = 0;
                }
                else if (collision.side == Struct::Right) {
                    Position[0] = collision.pos[0] + collision.dim[0] + Dimension[0];
                    velocity[0] = 0;
                }
            }
        }
    }
    else {
        velocity[1] = -3.0;
        Position[1] += velocity[1] * deltaTime;
    }
    
}

/// <summary>
/// Afficher le joueur avec la texture elle prend en argument la position et les dimensions en 2D du joueur
/// </summary>
/// <params name="position">Positoin du joueur</params>
/// <params name="position">Dimension du Joueur </params>

void  Mario::drawPlayerWithTexture(vector<float> pos, vector<float> dim) {

    glColor3f(1.0f, 1.0f, 1.0f);
    // Enable texture coordinates
    glEnable(GL_TEXTURE_2D);

    // Bind the texture
     
    if (gameOver) {
        glBindTexture(GL_TEXTURE_2D, textureDeadMario);
         } else 
    
        if (isLastPositionLeft) {
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

/// <summary>
    /// Affichage du score
    /// </summary>
void Mario::displayScore()
{
    std::string score_str = std::to_string(score);
    glRasterPos2f(0, 18);
    for (int i = 0; i < score_str.length(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, score_str[i]);
    }
}
/// <summary>
/// Tester la collision entre deux objets du jeu  elle prent en argument la position et les dimensions en 2D de ces deux objets et retourne un booléan qui indique si ces deux objets sont en collision ou pas
/// </summary>
/// <param name="PlayerPos">Position du 1er object</param>
/// <param name="PlayerDim">Dimension   du 1er object</param>
/// <param name="platformPos">Position du 2ème objet</param>
///<param name="platformDim">Dimension du 2ème  objet</param>
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

/// <summary>
/// Tester la collision entre le joueur et un objet prend en argumetns la position et les dimensions en 2D de cette   objet et retourne un booléan qui indique si ces deux entrent en collision ou pas
/// </summary>
/// <param name="targetPos">Position du l'objet</param>
/// <param name="targetDims">Dimensionde l'objet</param>

bool Mario::DetectCollisionWPlayer(vector<float> targetPos, vector<float> targetDims)
{
    //chack these one ...............
    return Position[1] - Dimension[1] < targetPos[1] + targetDims[1] &&
        Position[0] + Dimension[0] > targetPos[0] - targetDims[0] &&
        Position[0] - Dimension[0] < targetPos[0] + targetDims[0] &&
        Position[0] + Dimension[0] > targetPos[0] - targetDims[0];
}

/// <summary>
/// Tester la collision entre l'enemy et la plateforme prend en argment  la position et les dimensions en 2D de cette   objet et la position et les dimentions de la plateforme et retourne un CollisionSide qiu indique le sens de collision
/// </summary>
/// <param name="playerPos">Position du l'enemy</param>
/// <param name="playerDim">Dimensionde l'enemy</param>
/// /// <param name="platformPos">Position de la plateforme</param>
/// <param name="platformDim">Dimensionde la plateforme</param>

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
/// <summary>
/// Tester la collision entre le joueur et un objet de type Gold prend en argumetns la position et les dimensions en 2D de cette   objet et retourne un booléan qui indique si ces deux entrent en collision ou pas
/// </summary>
/// <param name="goldPos">Position du l'objet gold</param>
/// <param name="radius">Dimensionde l'objet gold</param>
bool Mario::detectGoldCols(vector<float> goldPos,float radius) {
    
        Struct::Collision collision = checkCollision( Position,Dimension,goldPos, { radius, radius });
        if (collision.side != Struct::None) {
            
            score += 1;
            //coinSound->play2D(coinSoundPath, false);
            cout << "your score : " << score << endl;
            return true;
        }
        return false;
    
}
/// <summary>
/// Tester la collision entre le joueur etet un Ennemy prend en argumetns la position et les dimensions en 2D de l'enemy et retourne un booléan qui indique si ces deux entrent en collision ou pas
/// </summary>
/// <param name="EnnemyPos">Position de l'enemy</param>
/// <param name="dim">Dimension de l'enemy</param>

int Mario::detectEnnemyCols(vector<float> EnnemyPos, vector<float> dim) {
    
        Struct::Collision collision = checkCollision(Position, Dimension, EnnemyPos, { dim[0],dim[1] });
        if (collision.side != Struct::None) {

            if (collision.side == Struct::Top) {
                cout << "top" << endl;
                return 1;
            }
            else  if (collision.side == Struct::Right) {
                gameOver = true;
                cout << "right" << endl;
                return 2;
            }
            else if ((collision.side == Struct::Left)) {
                cout << "left" << endl;
                gameOver = true;
                return 2;
            }
            else if ((collision.side == Struct::Bottom)) {
                cout << "bottom" << endl;
                gameOver = true;
                return 2;
            }

            return 0;
        }
        else {
            return 0;
        }
    
}
/// <summary>
/// Télecharger la strecture du joueur 
/// </summary>
/// <param name="a"></param>
/// <param name="fileName">Le fichier qui contient la strecture</param>
void Mario::loadTexture(int a, const char* fileName) {
    int width, height, numComponents;
    unsigned char* imageData = stbi_load(fileName, &width, &height, &numComponents, 0);

    if (imageData == NULL)
        std::cerr << "Unable to load texture: " << fileName << std::endl;
    else {
        cout << "worked" << endl;
        cout << "gold :" << width << endl;
        cout << "gold :" << height << endl;
        cout << "gold :" << numComponents << endl;

    }
    // Generate the texture object
    if (a == 0) {
        glGenTextures(1, &textureMarioLeft);
        glBindTexture(GL_TEXTURE_2D, textureMarioLeft);
    }
    else if (a == 1) {
        glGenTextures(1, &textureMarioRight);
        glBindTexture(GL_TEXTURE_2D, textureMarioRight);
    }
    else {
        glGenTextures(1, &textureDeadMario);
        glBindTexture(GL_TEXTURE_2D, textureDeadMario);
    }
    
    

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