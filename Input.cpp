#include "Input.h"
#include "Camera.h"
#include "UI/UI.h"
#include <GL/freeglut.h>
#include <algorithm>

bool keys[256] = { false };

int lastMouseX = 400, lastMouseY = 300;
bool firstMouse = true;

float mouseSensitivity = 0.2f;

extern float yaw;
extern float pitch;

// Camera mode = FPS-style look/move, UI mode = cursor-based button interaction.
static ControlMode g_controlMode = ControlMode::Camera;

ControlMode getControlMode()
{
    return g_controlMode;
}

void getMousePosition(int& x, int& y)
{
    x = lastMouseX;
    y = lastMouseY;
}

static void toggleControlMode()
{
    g_controlMode = (g_controlMode == ControlMode::Camera) ? ControlMode::UI : ControlMode::Camera;
    firstMouse = true;
    if (g_controlMode == ControlMode::UI) {
        glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    } else {
        glutSetCursor(GLUT_CURSOR_NONE);
    }
}

void keyDown(unsigned char key, int, int)
{
    keys[key] = true;
    if (key == '\t') {
        toggleControlMode();
    }
}

void keyUp(unsigned char key, int, int)
{
    keys[key] = false;
}

void mouseMotion(int x, int y)
{
    // In UI mode, keep raw cursor position for button hit testing / pizza hover.
    if (g_controlMode == ControlMode::UI) {
        lastMouseX = x;
        lastMouseY = y;
        return;
    }

    // Ignore first delta after mode switch to avoid abrupt camera jump.
    if (firstMouse) {
        lastMouseX = x;
        lastMouseY = y;
        firstMouse = false;
        return;
    }

    float offsetX = (x - lastMouseX) * mouseSensitivity;
    float offsetY = (lastMouseY - y) * mouseSensitivity;

    yaw += offsetX;
    pitch += offsetY;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    // Clamp + warp near edges so camera look remains continuous.
    int w = glutGet(GLUT_WINDOW_WIDTH);
    int h = glutGet(GLUT_WINDOW_HEIGHT);
    const int margin = 2;
    int cx = x;
    int cy = y;
    if (w > 2 * margin && h > 2 * margin) {
        if (x < margin || x >= w - margin || y < margin || y >= h - margin) {
            cx = std::clamp(x, margin, w - 1 - margin);
            cy = std::clamp(y, margin, h - 1 - margin);
            glutWarpPointer(cx, cy);
        }
    }

    lastMouseX = cx;
    lastMouseY = cy;
}

static bool g_pizzaClickPending = false;

bool consumePizzaClick()
{
    // One-shot event consumed by slice animation logic in main loop.
    if (g_pizzaClickPending) {
        g_pizzaClickPending = false;
        return true;
    }
    return false;
}

void mouseButton(int button, int state, int x, int y)
{
    // First route click to UI controls, then optionally flag pizza click.
    uiOnMouseClick(x, y, button, state);
    if (g_controlMode == ControlMode::UI && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        g_pizzaClickPending = true;
    }
}
