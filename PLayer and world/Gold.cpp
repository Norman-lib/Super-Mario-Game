
#include "Gold.h" 

Gold::Gold() {}


void Gold::drawGold(vector<float> g) {


		glBegin(GL_TRIANGLE_FAN);
		glVertex2f(goldPos[0], goldPos[1]);
		for (int i = 0; i <= 360; i++) {
			float rad = i * 3.14159 / 180;
			glVertex2f(goldPos[0] + cos(rad) * radius, goldPos[1] + sin(rad) * radius);
		}
		glEnd();
	
}


