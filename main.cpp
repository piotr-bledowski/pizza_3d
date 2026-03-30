#include <GL/freeglut.h>
#include <cmath>

// Global parameters
constexpr float PI = 3.14159265358979323846f;

// Pizza parameters
float g_radius = 2.5f;
float g_height = 0.3f;
int   g_segments = 32;
float g_edgeRadius = 0.15f;   // size of rounded edge
int   g_edgeSegments = 16;    // smoothness of rounding

// Camera parameters
float camX = 2.0f, camY = 2.0f, camZ = 5.0f;
float yaw = -90.0f;
float pitch = 0.0f;

float moveSpeed = 0.1f;
float mouseSensitivity = 0.2f;

// Key bindings
unsigned char cameraKeyForward = 'w';
unsigned char cameraKeyBackward = 's';
unsigned char cameraKeyLeft = 'a';
unsigned char cameraKeyRight = 'd';
unsigned char cameraKeyUp = 'q';
unsigned char cameraKeyDown = 'e';

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
    float innerR = radius - g_edgeRadius;
    float step = 2.0f * PI / segments;

    // ===== Flat top =====
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, halfH, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = i * step;
        glVertex3f(cos(theta) * innerR, halfH, sin(theta) * innerR);
    }
    glEnd();

    // ===== Flat bottom =====
    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, -halfH, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        float theta = i * step;
        glVertex3f(cos(theta) * innerR, -halfH, sin(theta) * innerR);
    }
    glEnd();

    // ===== Rounded edge (torus strip) =====
    for (int j = 0; j < g_edgeSegments; ++j) {
        float phi0 = (j / (float)g_edgeSegments) * PI - PI / 2.0f;
        float phi1 = ((j + 1) / (float)g_edgeSegments) * PI - PI / 2.0f;

        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= segments; ++i) {
            float theta = i * step;

            // Ring center
            float cx = cos(theta) * innerR;
            float cz = sin(theta) * innerR;

            // First ring
            float x0 = cx + cos(theta) * cos(phi0) * g_edgeRadius;
            float y0 = sin(phi0) * g_edgeRadius;
            float z0 = cz + sin(theta) * cos(phi0) * g_edgeRadius;

            // Second ring
            float x1 = cx + cos(theta) * cos(phi1) * g_edgeRadius;
            float y1 = sin(phi1) * g_edgeRadius;
            float z1 = cz + sin(theta) * cos(phi1) * g_edgeRadius;

            glVertex3f(x0, y0, z0);
            glVertex3f(x1, y1, z1);
        }
        glEnd();
    }
}

// Camera update
void updateCamera() {
    float radYaw = yaw * PI / 180.0f;
    float radPitch = pitch * PI / 180.0f;

    float dirX = cos(radYaw) * cos(radPitch);
    float dirY = sin(radPitch);
    float dirZ = sin(radYaw) * cos(radPitch);

    // Forward / backward
    if (keys[cameraKeyForward]) {
        camX += dirX * moveSpeed;
        camY += dirY * moveSpeed;
        camZ += dirZ * moveSpeed;
    }
    if (keys[cameraKeyBackward]) {
        camX -= dirX * moveSpeed;
        camY -= dirY * moveSpeed;
        camZ -= dirZ * moveSpeed;
    }

    // Strafe
    float rightX = -sin(radYaw);
    float rightZ = cos(radYaw);

    if (keys[cameraKeyLeft]) {
        camX -= rightX * moveSpeed;
        camZ -= rightZ * moveSpeed;
    }
    if (keys[cameraKeyRight]) {
        camX += rightX * moveSpeed;
        camZ += rightZ * moveSpeed;
    }

    // Vertical movement
    if (keys[cameraKeyUp]) {
        camY += moveSpeed;
    }
    if (keys[cameraKeyDown]) {
        camY -= moveSpeed;
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