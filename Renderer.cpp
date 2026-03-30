#include "Renderer.h"
#include "Camera.h"
#include "Mesh/Cylinder.h"
#include "Mesh/Cube.h"
#include <GL/freeglut.h>


// Objects (assigned from main)
Cylinder* g_cylinder = nullptr;
Cube* g_cube = nullptr;

void initGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void renderScene() {
    updateCamera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    applyCameraView();

    // Draw cylinder at origin
    if (g_cylinder) g_cylinder->draw();

    // Draw cube above cylinder
    if (g_cube) {
        glPushMatrix();
        glTranslatef(0.0f, g_cylinder->height * 0.5f + g_cube->size, 0.0f);
        g_cube->draw();
        glPopMatrix();
    }

    glutSwapBuffers();
    glutPostRedisplay();
}