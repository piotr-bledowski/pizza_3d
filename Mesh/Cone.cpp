#include "Cone.h"
#include <GL/freeglut.h>
#include <cmath>

namespace {
constexpr float PI = 3.14159265358979323846f;
}

Cone::Cone(float r, float h, int s) : radius(r), height(h), segments(s) {}

void Cone::draw() {
    const float halfH = height * 0.5f;
    const float step = 2.0f * PI / static_cast<float>(segments);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, -halfH, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        const float t = static_cast<float>(i) * step;
        const float x = radius * std::cos(t);
        const float z = radius * std::sin(t);
        glVertex3f(x, -halfH, z);
    }
    glEnd();

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < segments; ++i) {
        const float t0 = static_cast<float>(i) * step;
        const float t1 = static_cast<float>(i + 1) * step;
        const float x0 = radius * std::cos(t0);
        const float z0 = radius * std::sin(t0);
        const float x1 = radius * std::cos(t1);
        const float z1 = radius * std::sin(t1);
        glVertex3f(0.0f, halfH, 0.0f);
        glVertex3f(x0, -halfH, z0);
        glVertex3f(x1, -halfH, z1);
    }
    glEnd();
}
