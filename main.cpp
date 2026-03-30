#include <GL/freeglut.h>
#include "Renderer.h"
#include "Input.h"
#include "Mesh/Cylinder.h"
#include "Mesh/Cube.h"


// Global parameters (user-controlled)
float g_radius = 2.5f;
float g_height = 0.3f;
int g_segments = 32;

float g_cubeSize = 0.5f;

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Scene");

    // Create objects using parameters
    g_cylinder = new Cylinder(g_radius, g_height, g_segments);
    //g_cube = new Cube(g_cubeSize);

    initGL();

    glutDisplayFunc(renderScene);
    glutKeyboardFunc(keyDown);
    glutKeyboardUpFunc(keyUp);
    glutPassiveMotionFunc(mouseMotion);

    glutSetCursor(GLUT_CURSOR_NONE);

    glutMainLoop();
    return 0;
}