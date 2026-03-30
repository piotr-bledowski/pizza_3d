#include <GL/freeglut.h>
#include "Renderer.h"
#include "Input.h"

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Pizza");

    initGL();

    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);

    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}