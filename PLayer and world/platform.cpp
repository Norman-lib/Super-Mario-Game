#include "platform.h"


platform::platform(vector<float> pos, vector<float> dim) {
    this->pos = pos;
    this->dim = dim;
}

void platform::draw() {
    glBegin(GL_QUADS);
    glVertex2f(pos[0] - dim[0], pos[1] - dim[1]);
    glVertex2f(pos[0] - dim[0], pos[1] + dim[1]);
    glVertex2f(pos[0] + dim[0], pos[1] + dim[1]);
    glVertex2f(pos[0] + dim[0], pos[1] - dim[1]);
    glEnd();
}

vector<float> platform::getDim() {
    return dim;
}

vector<float> platform::getPos() {
    return pos;
}

