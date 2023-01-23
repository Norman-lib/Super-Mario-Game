#include "flag.h"

flag::flag(vector<float> pos, vector<float> dim) {
    this->pos = pos;
    this->dim = dim;
}

vector<float> flag::getDim() {
    return dim;
}

vector<float> flag::getPos() {
    return pos;
}


void flag::drawWithTexture() {

    glColor3f(1.0f, 1.0f, 1.0f);
    // Enable texture coordinates
    glEnable(GL_TEXTURE_2D);

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture);

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

void flag::loadTexture() {
    int width, height, numComponents;
 
    unsigned char* imageData = stbi_load("flag.jpg", &width, &height, &numComponents, 0);

    if (imageData == NULL)
        std::cerr << "Unable to load texture: " << "flag.png" << std::endl;
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