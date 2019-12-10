#include <GL/glut.h>
#include <iostream>
#include <utility>
#include <vector>
#include <array>
#include <cstdlib>

using namespace std;


void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glutSwapBuffers();
    glutPostRedisplay();
}


int main(int argc, char **argv)
{
    srand(10);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(500, 500);
    glutCreateWindow("BestRsProject");

    glutDisplayFunc(renderScene);

    glutMainLoop();

    return 0;
}
