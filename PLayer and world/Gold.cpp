
#include "Gold.h" 
#include "stb_image.h"


Gold::Gold() {
    dim = { 0.5, 0.5 };
}

Gold::Gold(vector<float> pos) {
    goldPos = pos;
    dim = { 0.5, 0.5 };
}


void Gold::drawGold(vector<float> g) {

    //glEnable(GL_TEXTURE_2D);

    //// Bind the texture
    //glBindTexture(GL_TEXTURE_2D, textureCoins);


		/*glBegin(GL_TRIANGLE_FAN);
		glVertex2f(goldPos[0], goldPos[1]);
		for (int i = 0; i <= 360; i++) {
			float rad = i * 3.14159 / 180;
			glVertex2f(goldPos[0] + cos(rad) * radius, goldPos[1] + sin(rad) * radius);
		}
		glEnd();*/

        glColor3f(1.0f, 1.0f, 1.0f);
        // Enable texture coordinates
        glEnable(GL_TEXTURE_2D);

        // Bind the texture
        glBindTexture(GL_TEXTURE_2D, textureCoins);
        //cout << textureCoins << endl; 

        // Draw the shape
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f, 0.0f);
        glVertex2f(g[0] + dim[0], g[1] + dim[1]);
        glTexCoord2f(1.0f, 0.0f);
        glVertex2f(g[0] - dim[0], g[1] + dim[1]);
        glTexCoord2f(1.0f, 1.0f);
        glVertex2f(g[0] - dim[0], g[1] - dim[1]);
        glTexCoord2f(0.0f, 1.0f);
        glVertex2f(g[0] + dim[0], g[1] - dim[1]);
        glEnd();
        // Unbind the texture
        glBindTexture(GL_TEXTURE_2D, 0);
	
}

void Gold::loadTexture(const char* filename) {
    int width, height, numComponents;
    unsigned char* imageData = stbi_load(filename, &width, &height, &numComponents, 0);

    if (imageData == NULL)
        std::cerr << "Unable to load texture: " << filename << std::endl;
    else {
        cout << "worked" << endl;
        cout << "gold :" << width << endl;
        cout << "gold :" << height << endl;
        cout << "gold :" << numComponents << endl;

    }
    // Generate the texture object
    glGenTextures(1, &textureCoins);
    glBindTexture(GL_TEXTURE_2D, textureCoins);
    cout << textureCoins << endl;

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


