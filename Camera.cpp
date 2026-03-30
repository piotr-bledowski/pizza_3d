#include "Camera.h"
#include "Input.h"
#include <GL/freeglut.h>
#include <cmath>


constexpr float PI = 3.14159265358979323846f;

// Camera state
float camX = 2.0f, camY = 2.0f, camZ = 5.0f;
float yaw = -90.0f;
float pitch = 0.0f;

float moveSpeed = 0.1f;

// Key bindings
unsigned char cameraKeyForward = 'w';
unsigned char cameraKeyBackward = 's';
unsigned char cameraKeyLeft = 'a';
unsigned char cameraKeyRight = 'd';
unsigned char cameraKeyUp = 'q';
unsigned char cameraKeyDown = 'e';

void updateCamera() {
    float radYaw = yaw * PI / 180.0f;
    float radPitch = pitch * PI / 180.0f;

    float dirX = cos(radYaw) * cos(radPitch);
    float dirY = sin(radPitch);
    float dirZ = sin(radYaw) * cos(radPitch);

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

    if (keys[cameraKeyUp]) {
        camY += moveSpeed;
    }
    if (keys[cameraKeyDown]) {
        camY -= moveSpeed;
    }
}

void applyCameraView() {
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
}