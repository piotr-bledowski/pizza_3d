#pragma once

extern bool keys[256];

enum class ControlMode { Camera, UI };

ControlMode getControlMode();

void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void specialKeyDown(int key, int x, int y);
void mouseMotion(int x, int y);
void mouseButton(int button, int state, int x, int y);
