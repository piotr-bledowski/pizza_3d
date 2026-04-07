#include "Sphere.h"
#include <GL/freeglut.h>

Sphere::Sphere(float r, int sl, int st) : radius(r), slices(sl), stacks(st) {}

void Sphere::draw() {
    glDisable(GL_TEXTURE_2D);
    glutSolidSphere(static_cast<double>(radius), slices, stacks);
}
