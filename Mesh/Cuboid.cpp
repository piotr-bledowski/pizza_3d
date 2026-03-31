#include "Cuboid.h"
#include <GL/freeglut.h>

Cuboid::Cuboid(float w, float h, float d) : width(w), height(h), depth(d) {}

void Cuboid::draw() {
    const float hx = width * 0.5f;
    const float hy = height * 0.5f;
    const float hz = depth * 0.5f;

    glBegin(GL_QUADS);

    glVertex3f(-hx, -hy, hz);
    glVertex3f(hx, -hy, hz);
    glVertex3f(hx, hy, hz);
    glVertex3f(-hx, hy, hz);

    glVertex3f(-hx, -hy, -hz);
    glVertex3f(-hx, hy, -hz);
    glVertex3f(hx, hy, -hz);
    glVertex3f(hx, -hy, -hz);

    glVertex3f(-hx, -hy, -hz);
    glVertex3f(-hx, -hy, hz);
    glVertex3f(-hx, hy, hz);
    glVertex3f(-hx, hy, -hz);

    glVertex3f(hx, -hy, -hz);
    glVertex3f(hx, hy, -hz);
    glVertex3f(hx, hy, hz);
    glVertex3f(hx, -hy, hz);

    glVertex3f(-hx, hy, -hz);
    glVertex3f(-hx, hy, hz);
    glVertex3f(hx, hy, hz);
    glVertex3f(hx, hy, -hz);

    glVertex3f(-hx, -hy, -hz);
    glVertex3f(hx, -hy, -hz);
    glVertex3f(hx, -hy, hz);
    glVertex3f(-hx, -hy, hz);

    glEnd();
}
