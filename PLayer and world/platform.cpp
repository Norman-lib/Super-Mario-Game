#include "platform.h"


platform::platform(vector<float> posi, vector<float> dime) {
    pos = posi;
    dim = dime;
}

void platform::draw() {
    glEnable(GL_TEXTURE_2D);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);
    float ratio = dim[0] / dim[1];
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 0.0f);
    glVertex2f(pos[0] + dim[0], pos[1] + dim[1]);
    glTexCoord2f(ratio, 0.0f);
    glVertex2f(pos[0] - dim[0], pos[1] + dim[1]);
    glTexCoord2f(ratio, 1.0f);
    glVertex2f(pos[0] - dim[0], pos[1] - dim[1]);
    glTexCoord2f(0.0f, 1.0f);
    glVertex2f(pos[0] + dim[0], pos[1] - dim[1]);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}


vector<float> platform::getDim() {
    return dim;
}

vector<float> platform::getPos() {
    return pos;
}

void platform::loadTexture() {
    int width, height, numComponents;
   
    unsigned char* imageData = stbi_load("tile.jpg", &width, &height, &numComponents, 0);

    if (imageData == NULL)
        std::cerr << "Unable to load texture: " << "platform.png" << std::endl;
    else {
        cout << "worked" << endl;
        cout << "width :" << width << endl;
        cout << "width :" << height << endl;
        cout << "width :" << numComponents << endl;

    }
    // Generate the texture object
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    // Copy the image data to the texture object
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
    glBindTexture(GL_TEXTURE_2D, 0);
    // Free the image data
    stbi_image_free(imageData);
}

