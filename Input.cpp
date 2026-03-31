#include "Input.h"
#include "Camera.h"


bool keys[256] = { false };
static bool keysPressedOnce[256] = { false };

int lastMouseX = 400, lastMouseY = 300;
bool firstMouse = true;

float mouseSensitivity = 0.2f;

// Access camera rotation
extern float yaw;
extern float pitch;

void keyDown(unsigned char key, int, int) {
    keys[key] = true;
}

void keyUp(unsigned char key, int, int) {
    keys[key] = false;
}

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

bool isKeyPressedOnce(unsigned char key) {
    if (keys[key] && !keysPressedOnce[key]) {
        keysPressedOnce[key] = true;
        return true;
    }
    if (!keys[key]) {
        keysPressedOnce[key] = false;
    }
    return false;
}