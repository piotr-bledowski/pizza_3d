#include "Cube.h"
#include <GL/freeglut.h>


Cube::Cube(float s) : size(s) {}

void Cube::draw() {
    float h = size * 0.5f;

    glBegin(GL_QUADS);

    // Front
    glVertex3f(-h, -h, h);
    glVertex3f(h, -h, h);
    glVertex3f(h, h, h);
    glVertex3f(-h, h, h);

    // Back
    glVertex3f(-h, -h, -h);
    glVertex3f(-h, h, -h);
    glVertex3f(h, h, -h);
    glVertex3f(h, -h, -h);

    // Left
    glVertex3f(-h, -h, -h);
    glVertex3f(-h, -h, h);
    glVertex3f(-h, h, h);
    glVertex3f(-h, h, -h);

    // Right
    glVertex3f(h, -h, -h);
    glVertex3f(h, h, -h);
    glVertex3f(h, h, h);
    glVertex3f(h, -h, h);

    // Top
    glVertex3f(-h, h, -h);
    glVertex3f(-h, h, h);
    glVertex3f(h, h, h);
    glVertex3f(h, h, -h);

    // Bottom
    glVertex3f(-h, -h, -h);
    glVertex3f(h, -h, -h);
    glVertex3f(h, -h, h);
    glVertex3f(-h, -h, h);

    glEnd();
}