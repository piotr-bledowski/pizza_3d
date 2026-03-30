#include <GL/freeglut.h>
#include <cmath>

// Global parameters
float g_radius = 2.5f;
float g_height = 0.3f;
int   g_segments = 32;
constexpr float PI = 3.14159265358979323846f;

// Camera parameters
float camX = 2.0f, camY = 2.0f, camZ = 5.0f;
float yaw = -90.0f;   // left/right
float pitch = 0.0f;   // up/down

float moveSpeed = 0.1f;
float mouseSensitivity = 0.2f;

bool keys[256] = { false };
int lastMouseX = 400, lastMouseY = 300;
bool firstMouse = true;

// Initialisation
void initGL() {
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, 800.0 / 600.0, 0.1, 100.0);

    glMatrixMode(GL_MODELVIEW);
}

// Utility: draw cylinder
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

// Camera update
void updateCamera() {
    float radYaw = yaw * PI / 180.0f;
    float radPitch = pitch * PI / 180.0f;

    float dirX = cos(radYaw) * cos(radPitch);
    float dirY = sin(radPitch);
    float dirZ = sin(radYaw) * cos(radPitch);

    // Forward / backward
    if (keys['w']) {
        camX += dirX * moveSpeed;
        camY += dirY * moveSpeed;
        camZ += dirZ * moveSpeed;
    }
    if (keys['s']) {
        camX -= dirX * moveSpeed;
        camY -= dirY * moveSpeed;
        camZ -= dirZ * moveSpeed;
    }

    // Strafe
    float rightX = -sin(radYaw);
    float rightZ = cos(radYaw);

    if (keys['a']) {
        camX -= rightX * moveSpeed;
        camZ -= rightZ * moveSpeed;
    }
    if (keys['d']) {
        camX += rightX * moveSpeed;
        camZ += rightZ * moveSpeed;
    }
}

// Input: keyboard
void keyDown(unsigned char key, int, int) {
    keys[key] = true;
}
void keyUp(unsigned char key, int, int) {
    keys[key] = false;
}

// Input: mouse
void mouseMotion(int x, int y) {
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
    }

    float offsetX = (x - lastMouseX) * mouseSensitivity;
    float offsetY = (lastMouseY - y) * mouseSensitivity;

    lastMouseX = x;
    lastMouseY = y;

    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
}

// Scene rendering
void renderScene() {
    updateCamera();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float radYaw = yaw * PI / 180.0f;
    float radPitch = pitch * PI / 180.0f;

    float dirX = cos(radYaw) * cos(radPitch);
    float dirY = sin(radPitch);
    float dirZ = sin(radYaw) * cos(radPitch);

    gluLookAt(
        camX, camY, camZ,
        camX + dirX, camY + dirY, camZ + dirZ,
        0, 1, 0
    );

    drawCylinder(g_radius, g_height, g_segments);

    glutSwapBuffers();
    glutPostRedisplay();
}

// Entry point
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