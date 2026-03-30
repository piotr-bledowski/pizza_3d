#include <GL/freeglut.h>
#include <cmath>

// Global parameters
float g_radius = 2.5f;
float g_height = 0.3f;
int   g_segments = 32;
constexpr float PI = 3.14159265358979323846f;

// Initialisation
void initGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

void drawCylinder(float radius, float height, int segments) {
    float halfH = height * 0.5f;
    float step = 2.0f * PI / segments;

    // Side surface
    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        float theta = i * step;
        float x = cos(theta) * radius;
        float z = sin(theta) * radius;

        glVertex3f(x, -halfH, z);
        glVertex3f(x, halfH, z);
    }
    glEnd();

    // Top cap
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, halfH, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = i * step;
        glVertex3f(cos(theta) * radius, halfH, sin(theta) * radius);
    }
    glEnd();

    // Bottom cap
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, -halfH, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = i * step;
        glVertex3f(cos(theta) * radius, -halfH, sin(theta) * radius);
    }
    glEnd();
}

void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    // Simple camera
    gluLookAt(2, 2, 2, 0, 0, 0, 0, 1, 0);

    // Draw object
    drawCylinder(g_radius, g_height, g_segments);

    glutSwapBuffers();
}

// Entry point
int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(800, 600);
    glutCreateWindow("OpenGL Pizza");

    initGL();
    glutDisplayFunc(renderScene);

    glutMainLoop();
    return 0;
}