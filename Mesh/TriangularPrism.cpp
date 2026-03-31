#include "TriangularPrism.h"
#include <GL/freeglut.h>

TriangularPrism::TriangularPrism(float baseW, float triH, float d)
    : baseWidth(baseW), triangleHeight(triH), depth(d) {}

void TriangularPrism::draw() {
    const float w = baseWidth;
    const float h = triangleHeight;
    const float zd = depth * 0.5f;

    const float ay = -h / 3.0f;
    const float by = -h / 3.0f;
    const float cy = 2.0f * h / 3.0f;

    const float ax = -w * 0.5f;
    const float bx = w * 0.5f;
    const float cx = 0.0f;

    glBegin(GL_TRIANGLES);
    glVertex3f(ax, ay, -zd);
    glVertex3f(bx, by, -zd);
    glVertex3f(cx, cy, -zd);

    glVertex3f(bx, by, zd);
    glVertex3f(ax, ay, zd);
    glVertex3f(cx, cy, zd);
    glEnd();

    glBegin(GL_QUADS);
    glVertex3f(ax, ay, -zd);
    glVertex3f(ax, ay, zd);
    glVertex3f(bx, by, zd);
    glVertex3f(bx, by, -zd);

    glVertex3f(bx, by, -zd);
    glVertex3f(bx, by, zd);
    glVertex3f(cx, cy, zd);
    glVertex3f(cx, cy, -zd);

    glVertex3f(cx, cy, -zd);
    glVertex3f(cx, cy, zd);
    glVertex3f(ax, ay, zd);
    glVertex3f(ax, ay, -zd);
    glEnd();
}
