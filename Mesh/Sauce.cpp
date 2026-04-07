#include "Sauce.h"
#include <GL/freeglut.h>
#include <cmath>

namespace {
constexpr float PI = 3.14159265358979323846f;
}

Sauce::Sauce(float r, float h, int s) : radius(r), height(h), segments(s) {}

void Sauce::draw() {
    const float halfH = height * 0.5f;
    const float step = 2.0f * PI / static_cast<float>(segments);

    glDisable(GL_TEXTURE_2D);
    glColor3f(0.78f, 0.12f, 0.08f);

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, -halfH, 0.0f);
    for (int i = 0; i <= segments; ++i) {
        const float t = static_cast<float>(i) * step;
        glVertex3f(radius * std::cos(t), -halfH, radius * std::sin(t));
    }
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, halfH, 0.0f);
    for (int i = segments; i >= 0; --i) {
        const float t = static_cast<float>(i) * step;
        glVertex3f(radius * std::cos(t), halfH, radius * std::sin(t));
    }
    glEnd();

    glBegin(GL_QUAD_STRIP);
    for (int i = 0; i <= segments; ++i) {
        const float t = static_cast<float>(i) * step;
        const float x = radius * std::cos(t);
        const float z = radius * std::sin(t);
        glVertex3f(x, -halfH, z);
        glVertex3f(x, halfH, z);
    }
    glEnd();

    glColor3f(1.0f, 1.0f, 1.0f);
}
