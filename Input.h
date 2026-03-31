#pragma once

extern bool keys[256];

void keyDown(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseMotion(int x, int y);
bool isKeyPressedOnce(unsigned char key);