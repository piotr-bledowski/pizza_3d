#include "Pea.h"
#include <GL/freeglut.h>

Pea::Pea(float r, int sl, int st) : Sphere(r, sl, st) {}

void Pea::draw() {
    glColor3f(0.18f, 0.62f, 0.28f);
    Sphere::draw();
    glColor3f(1.0f, 1.0f, 1.0f);
}
